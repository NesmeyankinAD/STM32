/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include "stm32f4xx.h"

#define TIME 10000000

/*********************************************************************
Программа передаёт состояние кнопки по UART и при приёме 1 зажигает светодиод, при приёме 0 - гасит.
Во внешнем прерывании по кнопке отправляем данные по USART в терминал
В прерывании по приёму анализируем данные, принятые по USART из терминала
*/

volatile uint32_t g_seconds = 0;
uint32_t Recieve = 0;

void RCC_Init(void);

int main(void)
{

  SystemInit();
  RCC_Init(); // настройка тактирования

  
  RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;         // Включение тактирования блока SYSCFGEN
  SYSCFG -> EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PE; // Задаём линию (0, 1..15) и порт (А..Е) для прерывания
                                                  // в одном регистре SYSCFG_EXTICR[1..4] можно задать 4 прерывания

  __enable_irq(); // Глобальное разрешение прерываний (по умолчанию разрешено)

  EXTI -> PR |= EXTI_PR_PR4;      // Сброс бита запроса прерывания (перед инициализацией прерывания)
  EXTI -> IMR |= EXTI_IMR_MR4;    // Включение прерывания по четвёртой линии
  EXTI -> FTSR |= EXTI_FTSR_TR4;  // Настройка события - спад сигнала, срабатывание прерывания по спаду
  NVIC_EnableIRQ(EXTI4_IRQn);     // Разрешение прерывания в контроллере NVIC
                                  // названия прерываний находятся в структуре IRQn_Type
  
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Включение тактирования порта A
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  // Включение тактирования порта E

  GPIOA -> MODER |= GPIO_MODER_MODE6_0; // Установка режима работы пина GPIOA_6 на выход (LED1 D2)
  GPIOA -> MODER |= GPIO_MODER_MODE7_0; // Установка режима работы пина GPIOA_7 на выход (LED2 D3)
  GPIOE -> MODER &= ~GPIO_MODER_MODE4_1; // Установка режима работы пина GPIOE_4 на вход (KEY0)

  GPIOE -> PUPDR |= GPIO_PUPDR_PUPDR4_0; // Подтяжка пина GPIOE_4 к VCC

  GPIOA -> BSRR |= GPIO_BSRR_BS7;

  /*----USART1-----*/{
    //RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Включение тактирования порта A
    RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;   // Включение тактирования USART1
    
    /* Настройка скорости приёма/передачи baudrate (бит/с)
   (APB2_PRPH_CLK / BRR_REF)/16 = Mantissa.Fraction (M.F)
   BRR = M.(0x(F*16))

   Пример: BRR_REF = 19200 бит/с(бод)
   1. (50MHz/19200)/16 = 162.7604167
   2. F*16 = 12.167  - округляем до целого, F = 12
   3. M = 162 = 0xA2
   4. BRR = M.F = 0xA2C = 0d2604
   5. Проверка: 50MHz / 2604 = 19201.23 бит/с
   */
    USART1 -> BRR = 0xA2C;
    
    USART1 -> CR1 |= USART_CR1_RXNEIE;    /* Включение прерывания по приёму.
                                     Прерывание активируется, когда приём байта окончен -
                                     данные записаны сначала в регистр RDR, затем в DR
                                     В прерывании по приёму можем взять данные из регистра DR*/
    USART1 -> CR1 |= USART_CR1_RE;        // Включение приёмника
    USART1 -> CR1 |= USART_CR1_TE;        // Включение передатчика
    USART1 -> CR1 &= ~(USART_CR1_M);      // Длина посылки 8, 1 старт, n стоп бит (n в CR2)
    USART1 -> CR1 &= ~(USART_CR1_PCE);    // Контроль чётности отключен
                                          
    USART1 -> CR2 &= ~(USART_CR2_STOP);   // n = 1 стоп бит
                                          
    NVIC_EnableIRQ(USART1_IRQn);          // Включение прерывания в контроллере NVIC
                                          
    USART1 -> CR1 |= USART_CR1_UE;        // Включение модуля USART1

    GPIOA -> MODER |= GPIO_MODER_MODE9_1; // Альтернативная функция пина 9
    GPIOA -> MODER |= GPIO_MODER_MODE10_1;// Альтернативная функция пина 10

    GPIOA -> AFR[1] |= (7 << GPIO_AFRH_AFSEL9_Pos); // Задание альт. функции №7 - USART для пина 9
    GPIOA -> AFR[1] |= (7 << GPIO_AFRH_AFSEL10_Pos);// Задание альт. функции №7 - USART для пина 10
  /*----USART1-----*/}

  /*----TIM2----*/{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Включение тактирования счётчика TIM2
    
    /* Пояснения к счётчику
  // (APB1_TIMER_CLK / (PSC+1)) / (ARR+1) = 1 Hz  - частота прерывания счётчика
  // PSC = 9999 -> 50e6 / 10000 = 5000 Hz         - делитель частоты шины APB1
  // ARR = 4999 -> 5000 / 5000 = 1 Hz             - счётчик тиков для сброса счётчика в 0
  // (50 МГц / (10000)) / (5000) = 1 Hz
  */
    
    TIM2->PSC = 9999; 
    TIM2->ARR = 4999;
    TIM2->EGR |= TIM_EGR_UG;
    
    TIM2->SR = 0;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_SetPriority(TIM2_IRQn, 0); 
    NVIC_EnableIRQ(TIM2_IRQn);
  }/*----TIM2----*/

  while(1)
  {   
    uint32_t j = 0;

    GPIOA -> BSRR |= GPIO_BSRR_BR6; // Светодиод включен
    for(j = 0;j < TIME;j++){}       // Задержка
    GPIOA -> BSRR |= GPIO_BSRR_BS6; // Светодиод выключен
    for(j = 0;j < TIME;j++){}       // Задержка
  }

  
}

void EXTI4_IRQHandler(void)
{
//--------Программа обработчика прерывания----------------------//

  USART1 -> DR = 1; // Как только данные будут записаны в регситр DR
                    // начнётся запись данных в регистр TDR и произойдёт передача

//--------Программа обработчика прерывания----------------------//


  EXTI -> PR |= EXTI_PR_PR4; // Сброс бита запроса прерывания 
                             // (после выполнения программы обработчика прерывания)
}

void USART1_IRQHandler(void)
{
  // Здесь нужна проверка, по какому флагу мы зашли в общее прерывание по USART1

  if ((USART1 -> SR & USART_SR_RXNE) != 0) // Прерывание по приёму USART_CR_RXNEIE
  {

    Recieve = USART1 -> DR;//debug

    uint16_t RecievedData = (uint16_t)(USART1 -> DR & (uint16_t)0x1FF); // Приводим 32-разрядный DR к 16-разрядному RecievedData
                                                                          // и отсекаем лишние биты (полезные данные только в битах 0..8)
    switch(RecievedData)
    {
      case '0': 
        GPIOA -> BSRR |= GPIO_BSRR_BR7;
        break;

      case '1': 
        GPIOA -> BSRR |= GPIO_BSRR_BS7;
        break;

      default: break;
    }
  }

  NVIC_ClearPendingIRQ(USART1_IRQn); // Сброс запроса прерывания
}

void TIM2_IRQHandler(void) 
{ 
  if (TIM2->SR & TIM_SR_UIF) 
  { 
  // обновление произошло 
  TIM2->SR &= ~TIM_SR_UIF; // очистить флаг обновления
  
  g_seconds++;
  }
}
/*************************** End of file ****************************/
