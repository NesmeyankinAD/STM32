#include "stm32f4xx.h"

/*
Плавное мигание светодиодом D3 (GPIOA_7) осуществляется с помощью 
канала CH1N (комплиментарный CH1) счётчика TIM8.
*/

void TIM8_Init(void)
{
  /*---Тактирование и порты---*/
  RCC -> APB2ENR |= RCC_APB2ENR_TIM8EN;             //Тактирование таймера TIM8
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;            //Тактирование порта GPIOA
                                                    
  GPIOA -> MODER   |= GPIO_MODER_MODE7_1;           //Альтернативная функция GPIOA_7
  GPIOA -> OSPEEDR |= GPIO_OSPEEDR_OSPEED7_1;       //High speed GPIOA_7
  GPIOA -> AFR[0]  |= (3 << GPIO_AFRL_AFSEL7_Pos);  //AF3 TIM8_CH1N у GPIOA_7
  

  /*---Настройка таймера TIM8---*/
  /*
  1. Частота тактирования таймера
                APB2_FREQ
  TIM_FREQ = ---------------
              Prescaler_ref

  TIM -> PSC = Prescaler_ref - 1 (так как счёт от нуля)
  
  2. Частота тиков таймера (частота ШИМ)
     Это частота генерации сигнала таймером. Таймер считает количество
  тактовых импульсов TIM_FREQ, записанное в регистре ARR (счёт от нуля). 
  Получаем частоту (период) выходного сигнала таймера равный
                  TIM_FREQ
  TIM_TICK_FREQ = ---------
                   ARR_REF
  
  TIM -> ARR = ARR_REF - 1 (так как счёт от нуля)

  3. Для настройки ШИМ в регистре CCMRx выбираем режим ШИМ (PWM mode 1 или 2).
  В этом режиме производится сравнение выходного значения счётчика со значением, 
  записанным на момент сравнения в регистре CCRx.
  */

  TIM8 -> PSC    = 84-1;              //Prescaler; частота тактирования 1МГц 
                                      
  TIM8 -> CR1   |= TIM_CR1_CMS;       //Режим выравнивания по центру
                                      
  TIM8 -> ARR    = 1000-1;            //Частота ШИМ 1кГц
                                      
  TIM8 -> CCR1   = 0;                 //Duty cycle, будем изменять в программе, 
                                      //просто инициализируем единицей
                                      
  TIM8 -> CCMR1 |= TIM_CCMR1_OC1M;    //PWM Mode 2 для CH1
  TIM8 -> CCMR1 &= ~(TIM_CCMR1_CC1S); //Режим работы CH1 на выход

  TIM8 -> CCER  |= TIM_CCER_CC1E;     //Включаем канал CH1, выход OC1
  TIM8 -> CCER  |= TIM_CCER_CC1NE;    //Включаем канал CH1, выход OC1N (комплиментарный OC1)

  TIM8 -> BDTR  |= TIM_BDTR_MOE;      //Разрешение работы OC1 и OC1N в блоке dead-time

  TIM8 -> CR1   |= TIM_CR1_CEN;       //Включение таймера
  
  TIM8 -> EGR   |= TIM_EGR_UG;        //Вызов события обновления для обновления
                                      //значений регистров ARR и CCR1

}