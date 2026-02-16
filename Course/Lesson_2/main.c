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
*
*       main()
*
*  Function description
*   Application entry point.
*/

volatile uint32_t g_seconds = 0;

void func(void)
{
  uint16_t f = 0;

  f++;
};

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

  /*----TIM2----*/
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
  /*----TIM2----*/

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
    uint32_t i = 0;

    GPIOA -> BSRR |= GPIO_BSRR_BR7; // Светодиод включен
    for(i = 0;i < TIME/10 ;i++){}   // Задержка
    GPIOA -> BSRR |= GPIO_BSRR_BS7; // Светодиод выключен
    for(i = 0;i < TIME/10 ;i++){}   // Задержка

    GPIOA -> BSRR |= GPIO_BSRR_BR7; // Светодиод включен
    for(i = 0;i < TIME/10 ;i++){}   // Задержка
    GPIOA -> BSRR |= GPIO_BSRR_BS7; // Светодиод выключен
    for(i = 0;i < TIME/10 ;i++){}   // Задержка

    GPIOA -> BSRR |= GPIO_BSRR_BR7; // Светодиод включен
    for(i = 0;i < TIME/10 ;i++){}   // Задержка
    GPIOA -> BSRR |= GPIO_BSRR_BS7; // Светодиод выключен
    for(i = 0;i < TIME/10 ;i++){}   // Задержка
//--------Программа обработчика прерывания----------------------//


    EXTI -> PR |= EXTI_PR_PR4;      // Сброс бита запроса прерывания 
                                    // (после выполнения программы обработчика прерывания)
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
