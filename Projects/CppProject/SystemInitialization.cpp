#include "SystemInitialization.h"

void ADC1_Init()
{
  /* ADC1 IN2 - GPIO A2 */

  /*---Тактирование и порты АЦП---*/
  {
    RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;    //Тактирование АЦП1
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   //Тактирование GPIOA АЦП1
    
    GPIOA -> MODER |= GPIO_MODER_MODE2;      //GPIO_A2 - аналоговый режим
  }
  
  /*---Настройка АЦП---*/
  {
    ADC1 -> SMPR2 |= ADC_SMPR2_SMP2_0;       //001 - 15 циклов тактирования АЦП (sample time)
                                             //Длительность одной конвертации 12 + sample time = 27 циклов тактирования АЦП

    ADC1 -> SQR1  &= ~(ADC_SQR1_L);          //Длина последовательности конвертаций = 1
    
    ADC1 -> SQR3  |= (2 << ADC_SQR3_SQ1_Pos);//Номер канала АЦП в последовательности конвертаций SQR
                                             //Канал №2 АЦП1 первый в последовательности конвертаций SQR
    
    ADC1 -> CR1   |= ADC_CR1_EOCIE;          //Разрешение прерывания по окончанию преобразования
    NVIC_EnableIRQ(ADC_IRQn);                //Разрешение прерываний в контроллере NVIC

    ADC1 -> CR2   |= ADC_CR2_CONT;           //Непрерывный режим работы АЦП1
    ADC1 -> CR2   |= ADC_CR2_EOCS;           //Прерывание EOCI вызывается после каждой конвертации в последовательности
    
    ADC1 -> CR2   |= ADC_CR2_ADON;           //Включение АЦП

    ADC1 -> CR2   |= ADC_CR2_SWSTART;        //Включение конвертаций АЦП
  }
}


void GPIO_Init()
{
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //Включение тактирования порта А 
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOEEN; //Включение тактирования порта Е


  /*------Кнопки-------*/
  GPIOE -> MODER &= ~GPIO_MODER_MODE4_1; // Установка режима работы пина GPIOE_4 на вход (KEY_0)
  GPIOE -> MODER &= ~GPIO_MODER_MODE3_1; // Установка режима работы пина GPIOE_3 на вход (KEY_1)
  GPIOA -> MODER &= ~GPIO_MODER_MODE0_1; // Установка режима работы пина GPIOA_0 на вход (KEY_UP)

  GPIOE -> PUPDR |= GPIO_PUPDR_PUPDR4_0; // Подтяжка пина GPIOE_4 к VCC
  GPIOE -> PUPDR |= GPIO_PUPDR_PUPDR3_0; // Подтяжка пина GPIOE_3 к VCC
  GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR0_1; // Подтяжка пина GPIOE_3 к GND

  /*------Светодиоды-------*/
  GPIOA -> MODER |= GPIO_MODER_MODE6_0;  // Установка режима работы пина GPIOA_6 на выход (LED D2)
  //GPIOA -> MODER |= GPIO_MODER_MODE7_0;  // Установка режима работы пина GPIOA_7 на выход (LED D3)

  GPIOA -> BSRR |= GPIO_BSRR_BS6;        // Выключение D2
  //GPIOA -> BSRR |= GPIO_BSRR_BS7;        // Выключение D3

    /*---Настройка GPIO_A7 AF1 - TIM1_CH1N---*/
    GPIOA -> MODER   |= GPIO_MODER_MODE7_1;          //Альтернативная функция GPIO_A7 (LED D3)
    GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;    //Highspeed GPIO_A7
    GPIOA -> AFR[0]  |= (1 << GPIO_AFRL_AFSEL7_Pos); //AF1 у GPIO_A7          

}


void RCC_Init()
{
  RCC -> CR |= RCC_CR_HSEON;                    // Включение внешнего источника тактового сигнала (кварц 8 МГц)
  while ( (RCC -> CR & RCC_CR_HSERDY) == 0 ) {} // Ожидание стабилизации тактового сигнала
                                                // (ожидание установки бита HSERDY в регистре CR)

  RCC -> CR &= ~(RCC_CR_PLLON);                 // Отключение PLL перед настройкой тактирования (явная установка 0)
  RCC -> PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;     // Выбор HSE в качестве источника тактирования

/* Пояснения к настройке тактирования 

F_HSE_CLK         = 8 MHz;  (кварцевый резонатор)
F_PLL_CLK_OUT_REF = 168 MHz;(требуемая выходная частота PLL)

F_VCO_CLK     = F_HSE_CLK * (PLLN / PLLM);
F_PLL_CLK_OUT = F_VCO_CLK / PLLP;

                                PLLN
F_PLL_CLK_OUT = F_HSE_CLK * -----------;
                            PLLP * PLLM

Ограничения:
F_PLL_CLK_OUT <= 168 MHz;
F_VCO_CLK     <= 432 MHz;

64 <= PLLN <= 432;
2  <= PLLM <= 63;
------
                    168
168 MHz = 8 MHz * -------;
                   2 * 4

  PLLP = 2;   (00)
  PLLN = 168; (1010 1000)
  PLLM = 4;   (00 0100)

SYSCLK = 168 MHz;
-----

HCLK = SYSCLK / AHB_PRESC;

APB1_PRPH_CLK  =  HCLK / APB1_PRESC;
APB1_PRPH_CLK  <= 42 MHz;
APB1_TIMER_CLK = APB1_PRPH_CLK * 2;

APB2_PRPH_CLK  =  HCLK / APB2_PRESC;
APB2_PRPH_CLK  <= 84 MHz;
APB2_TIMER_CLK = APB2_PRPH_CLK * 2;
-----

SYSCLK = 168 MHz;

AHB_PRESC = 1;

HCLK = 168 MHz / 1 = 168 MHz;

APB1_PRESC     = 4;
APB1_PRPH_CLK  = 168 MHz / 4 = 42 MHz; 
APB1_TIMER_CLK = 42 MHz * 2 = 84 MHz;

APB2_PRESC     = 2;
APB2_PRPH_CLK  = 168 MHz / 2 = 84 MHz;
APB2_TIMER_CLK = 84 MHz * 2  = 168 MHz;
----

Требуемые настройки тактирования:

RCC -> PLLCFG:
1. PLLP = 2;   (00)
2. PLLN = 168; (1010 1000)
3. PLLM = 4;   (00 0100)

RCC -> CFGR:
4. AHB_PRESC  = 1;
5. APB1_PRESC = 4;
6. APB2_PRESC = 2;

*/
 
/*------------------------------------------------------------------------------------------- */
    // PLLM = 000100;
    RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLM); // Очистка регистра в области PLLM
    RCC -> PLLCFGR |= RCC_PLLCFGR_PLLM_2;  // Запись 1 побитово для каждого разряда
    
    // PLLN = 1010 1000; 
    RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
    RCC -> PLLCFGR |= RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7;
    
    // PLLP = 00;
    RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
/*------------------------------------------------------------------------------------------- */
    // AHB Prescaler = 1;
    RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;
    
    // APB1 Prescaler = 4;
    RCC -> CFGR |= RCC_CFGR_PPRE1_DIV4;
    
    // APB2 Prescaler = 2;
    RCC -> CFGR |= RCC_CFGR_PPRE2_DIV2;
/*------------------------------------------------------------------------------------------- */

 RCC -> CR |= RCC_CR_PLLON; // Включение PLL

 while ( (RCC->CR & RCC_CR_PLLRDY) == 0 ) {} // Ожидание стабилизации частоты PLL

/* Настройка регистров FLASH-памяти.
Эта операция обязательна и выполняется только 1 раз при старте программы.*/
/*------------------------------------------------------------------------------------------- */
 FLASH -> ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS | FLASH_ACR_PRFTEN;
/*------------------------------------------------------------------------------------------- */
 
 RCC -> CFGR &= ~RCC_CFGR_SW;
 RCC -> CFGR |= RCC_CFGR_SW_PLL;  // Подключение PLL к SYSCLK.
                                  // Выбор PLL в качестве основного источника тактирования

 while ( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL ) {} // Ожидание, пока PLL подключится к SYSCLK

 SystemCoreClockUpdate();
}


void TIM1_Init()
{
  /*Abstract of TIM1
  TIM1 - Slave 16-bit up-down center align counter for TIM2
  PWM generation timer with complementary channels
  PE7  - ETR
  PE8  - CH1N
  PE9  - CH1
  PE10 - CH2N
  PE11 - CH2
  PE12 - CH3N
  PE13 - CH3
  PE14 - CH4
  PE15 - BKIN
  */
  
  /*---Тактирование и порты---*/
  {
  RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;               //Тактирование таймера TIM1
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOEEN;              //Тактирование порта GPIOE

  //GPIOE -> MODER |= GPIO_MODER_MODE7_1;             //Режим альтернативной функции для указанных пинов
  GPIOE -> MODER |= GPIO_MODER_MODE8_1;               
  GPIOE -> MODER |= GPIO_MODER_MODE9_1;               
  //GPIOE -> MODER |= GPIO_MODER_MODE10_1;            
  //GPIOE -> MODER |= GPIO_MODER_MODE11_1;            
  //GPIOE -> MODER |= GPIO_MODER_MODE12_1;            
  //GPIOE -> MODER |= GPIO_MODER_MODE13_1;            
  //GPIOE -> MODER |= GPIO_MODER_MODE14_1;            
  //GPIOE -> MODER |= GPIO_MODER_MODE15_1;            
                                                      
  //GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED7_1;       //High speed mode для указанных пинов
  GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1;         
  GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED9_1;         
  //GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED10_1;      
  //GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED11_1;      
  //GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED12_1;      
  //GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED13_1;      
  //GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED14_1;      
  //GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED15_1;      
                                                      
  //GPIOE -> AFR[0]  |= (1 << GPIO_AFRL_AFSEL7_Pos);  //AF1 у указанных пинов
  GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL8_Pos);
  GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL9_Pos);
  //GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL10_Pos);
  //GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL11_Pos);
  //GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL12_Pos);
  //GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL13_Pos);
  //GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL14_Pos);
  //GPIOE -> AFR[1]  |= (1 << GPIO_AFRH_AFSEL15_Pos);
  }
  
  /*---Настройка частоты таймера TIM1---*/
  {
  /*Пояснение
  1. Частота тактирования таймера:
                APB2_FREQ_TIM    84 * 2 MHz
  TIM_FREQ = ----------------- = ---------- = 56 MHz
                Prescaler_ref        3

  TIM -> PSC = Prescaler_ref - 1 (так как счёт от нуля)
  TIM -> PSC = 3 - 1

  2. Частота сигнала, получаемая с таймера (частота ШИМ)
     Это частота (1/период) двухскатной пилы, получаемой со счётчика. 
     Счётчик считает количество тактовых импульсов TIM_FREQ, записанное в регистре ARR (счёт от нуля)
     вверх до (ARR_REF/2)-1, и вниз до нуля. Всего ARR импульсов.
  
  Получаем удвоенную частоту (1/период) двухскатной пилы таймера TIM_TICK_FREQ равную:
                     TIM_FREQ       56 MHz
  2*TIM_TICK_FREQ = ----------- = --------- = 2 kHz (это удвоенная частота двускатной пилы)
                    (ARR_REF/2)    (56000/2)
  
  TIM_TICK_FREQ = 1kHz (это частота импульсов ШИМ)

  TIM -> ARR = (ARR_REF/2) - 1 (так как счёт от нуля)
  TIM -> ARR = 28000 - 1
  
  Получаем двухскатную пилу c верхним значением (ARR + 1) = 28000 и частотой TIM_TICK_FREQ = 1 kHz.

  3. Для настройки ШИМ в регистре CCMRx выбираем режим ШИМ (PWM mode 1).
  В этом режиме производится сравнение выходного значения счётчика со значением, 
  записанным на момент сравнения в регистре CCRx.
  |OCx = 1, if CNT < CCRx;
  |OCx = 0, if CNT > CCRx;
  |OCxN = !OCx.
  */

  TIM1 -> PSC    = 3-1;               //Prescaler; частота тактирования 42 МГц 
  TIM1 -> CR1   |= TIM_CR1_CMS;       //Режим выравнивания по центру
  TIM1 -> ARR    = 28000 - 1;         //Частота ШИМ 1кГц
  TIM1 -> CCR1   = 14000;             //Duty cycle, будем изменять в программе
  TIM1 -> CCR2   = 0;                 //Duty cycle, будем изменять в программе
  TIM1 -> CCR3   = 0;                 //Duty cycle, будем изменять в программе, 
  }
  
  /*---Настройка Slave-mode---*/
  //TIM1 -> SMCR |= TIM_SMCR_MSM;       //

  TIM1 -> SMCR |= TIM_SMCR_TS_0;      //001 - ITR1 TIM2-master
  
  TIM1 -> SMCR |= TIM_SMCR_SMS_2;     //100 - Slave-mode - Reset mode
                                      //счётчик стартует сам, но сбрасывается по триггеру
                                      //и происходит синхронизация
  
  /*---Настройка каналов TIM1---*/
  {
  //Канал CH1 - выход
  {
  TIM1 -> CCMR1 |= TIM_CCMR1_OC1M_1 
                |  TIM_CCMR1_OC1M_2;  //110 - PWM Mode 1 для CH1

  TIM1 -> CCMR1 &= ~(TIM_CCMR1_CC1S); //Режим работы CH1 на выход
  TIM1 -> CCMR1 |= TIM_CCMR1_OC1PE;   //Включение предзагрузки регистра CCR1

  TIM1 -> CCER  |= TIM_CCER_CC1E;     //Включаем канал CH1, выход OC1
  TIM1 -> CCER  |= TIM_CCER_CC1NE;    //Включаем канал CH1, выход OC1N (комплиментарный OC1)
  
  //Настройка полярности в CCER: CC1P=0(прямой) и CC1NP=1(инверсный) по умочланию
  }

  //Канал CH2 - выход
  {
  TIM1 -> CCMR1 |= TIM_CCMR1_OC2M_1 
                |  TIM_CCMR1_OC2M_2;  //110 - PWM Mode 1 для CH2

  TIM1 -> CCMR1 &= ~(TIM_CCMR1_CC2S); //Режим работы CH2 на выход
  TIM1 -> CCMR1 |= TIM_CCMR1_OC2PE;   //Включение предзагрузки регистра CCR2

  TIM1 -> CCER  |= TIM_CCER_CC2E;     //Включаем канал CH2, выход OC2
  TIM1 -> CCER  |= TIM_CCER_CC2NE;    //Включаем канал CH2, выход OC2N (комплиментарный OC2)
  
  //Настройка полярности в CCER: CC2P=0(прямой) и CC2NP=1(инверсный) по умочланию
  }

  //Канал CH3 - выход
  {
  TIM1 -> CCMR2 |= TIM_CCMR2_OC3M_1 
                |  TIM_CCMR2_OC3M_2;  //110 - PWM Mode 1 для CH3

  TIM1 -> CCMR2 &= ~(TIM_CCMR2_CC3S); //Режим работы CH3 на выход
  TIM1 -> CCMR2 |= TIM_CCMR2_OC3PE;   //Включение предзагрузки регистра CCR3

  TIM1 -> CCER  |= TIM_CCER_CC3E;     //Включаем канал CH3, выход OC3
  TIM1 -> CCER  |= TIM_CCER_CC3NE;    //Включаем канал CH3, выход OC3N (комплиментарный OC2)
  
  //Настройка полярности в CCER: CC3P=0(прямой) и CC3NP=1(инверсный) по умочланию
  }
  }
  
  TIM1 -> BDTR  |= TIM_BDTR_MOE;      //Разрешение работы OCx и OCxN в блоке dead-time
  
}//TIM1_Init()

void TIM2_Init()
{
  /*Abstract of TIM2
  TIM2 - Master 32-bit up counter for TIM1
  The calculation of the control system is performed 
  in an interrupt compared to the CNT and CCR registers of the TIM2
  */
  
  /*---Тактирование и порты---*/
  {
  RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN; //Тактирование таймера TIM2

    {///*---DEBUG---*/
    //RCC   -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;          //Тактирование GPIOA
    //GPIOA -> MODER   |= GPIO_MODER_MODE1_1;           //Режим альтернативной функции для указанных пинов
    //GPIOA -> OSPEEDR |= GPIO_OSPEEDR_OSPEED1_1;       //High speed mode для указанных пинов
    //GPIOA -> AFR[0]  |= (1 << GPIO_AFRL_AFSEL1_Pos);  //AF1 у указанных пинов
    ///*---DEBUG---*/
    }
  }
  
  /*---Настройка частоты таймера TIM2---*/
  {
  /*Пояснение
  1. Частота тактирования таймера:
              APB1_FREQ_TIM     2 * 42 MHz
  TIM_FREQ = --------------- = ------------ = 84 MHz
              Prescaler_ref         1

  TIM -> PSC = Prescaler_ref - 1 (так как счёт от нуля)
  TIM -> PSC = 1 - 1

  2. Частота сигнала, получаемая с таймера (частота ШИМ)
     Это частота (1/период) односкатной пилы (счёт вверх), получаемой со счётчика. 
     Счётчик считает количество тактовых импульсов TIM_FREQ, записанное в регистре ARR (счёт от нуля)
     вверх до ARR_REF. Всего ARR импульсов.
  Получаем частоту (1/период) односкатной пилы таймера равную
                  TIM_FREQ     84 MHz
  TIM_TICK_FREQ = --------- = -------- = 1 kHz
                   ARR_REF      84000
  
  TIM -> ARR = ARR_REF - 1 (так как счёт от нуля)
  TIM -> ARR = 84000 - 1
  
  Получаем односкатную пилу (счёт вверх) c верхним значением ARR + 1 = 84000 и частотой TIM_TICK_FREQ = 1 kHz.
  */

  TIM2 -> PSC    = 1-1;               //Prescaler; частота тактирования 42 МГц 
  TIM2 -> CR1   &= ~(TIM_CR1_CMS);    //Режим выравнивания edge align
  TIM2 -> CR1   |= TIM_CR1_DIR;       //Режим счёт вверх
  TIM2 -> ARR    = 84000 - 1;         //Частота пилы 1кГц
  TIM2 -> CCR2   = 42000;             //Duty cycle, момент начала расчёта
  }
  
  /*---Настройка Master-mode---*/

  TIM2 -> CR2 |=TIM_CR2_MMS_1;  //010 - Master mode - Update

  /*---Настройка каналов TIM2---*/
  {
      ///*---DEBUG---*/
      //  //PA1 - CH2
      //  //Канал CH2 - выход
      //{
      //TIM2 -> CCMR1 |= TIM_CCMR1_OC2M_1 
      //              |  TIM_CCMR1_OC2M_2;  //110 - PWM Mode 1 для CH2
      //
      //TIM2 -> CCMR1 &= ~(TIM_CCMR1_CC2S); //Режим работы CH2 на выход
      //TIM2 -> CCMR1 |= TIM_CCMR1_OC2PE;   //Включение предзагрузки регистра CCR2
      //
      //TIM2 -> CCER  |= TIM_CCER_CC2E;     //Включаем канал CH2, выход OC2
      //
      ////Настройка полярности в CCER: CC2P=0(прямой) и CC2NP=1(инверсный) по умочланию
      //}
      ///*---DEBUG---*/
  }

  /*---Настройка прерываний---*/
  {
    TIM2 -> DIER |= TIM_DIER_CC2IE; //Включение прерываний по каналу 2

    TIM2 -> SR   &= ~TIM_SR_CC2IF;  //Очистка флага запроса прерывания

    //NVIC_SetPriority(TIM2_IRQn, 1); //Установка приоритета прерывания в NVIC
    NVIC_EnableIRQ(TIM2_IRQn);      //Разрешение работы прерывания в NVIC
  }

  
}//TIM2_Init()

void DAC_Init()
{
  /*Abstract of DAC
  12-bit right-aligned mode DAC on pins PA4, PA5
  */

  RCC -> APB1ENR |= RCC_APB1ENR_DACEN;     //Тактирование DAC
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   //Тактирование GPIOA 

  GPIOA -> MODER |= GPIO_MODER_MODE4;      //Аналоговый режим GPIOA4 (DAC1_OUT)
  //GPIOA -> MODER |= GPIO_MODER_MODE5;      //Аналоговый режим GPIOA5 (DAC2_OUT)

  DAC -> CR |= DAC_CR_EN1;                 //Включить DAC канал 1
  //DAC -> CR |= DAC_CR_EN2;                 //Включить DAC канал 2
  DAC -> DHR12R1 = 0;                      //Инициализация выхода DAC1_OUT
  //DAC -> DHR12R2 = 0;                      //Инициализация выхода DAC2_OUT
}

void TIM_Start()
{
  //---TIM1:Slave mode - Reset mode; TIM2:Master mode - Update ---//

  TIM1 -> CR1   |= TIM_CR1_CEN;//Включение таймера-slave TIM1
  
  TIM2 -> CR1   |= TIM_CR1_CEN;//Включение таймера-master TIM2
  TIM2 -> EGR   |= TIM_EGR_UG; //Вызов события обновления для обновления
                               //значений регистров ARR и CCR2
                               //по этому событию произойдёт синхронизация счётчиков
}//TIM_Start()
