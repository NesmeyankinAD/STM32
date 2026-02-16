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

#define TIME 1000000

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/

uint16_t c = 0;

void func(void)
{
  uint16_t f = 0;

  f++;
};

int main(void)
{

  SystemInit();
  
  // Включение тактирования порта A
  // в модуле RCC выбрали регистр AHB1ENR и через OR установили 1 в AHB1ENR по адресу RCC_AHB1ENR_GPIOAEN
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  // Установка режима работы вывода GPIOA_6 на выход (LED_1 D2 на)
  // в модуле GPIOA выбрали регистр MODER и установили в нем 1 по адресу GPIO_MODER_MODE6_0 (0 - выход, 1 - вход)
  GPIOA -> MODER |= GPIO_MODER_MODE6_0;

  uint32_t i = 0;

  while(1)
  {
    
    // Светодиод включен
    // Установка 0 на выходе A6
    // в блоке GPIOA выбрали регистр BSRR (bit set-reset register) и установили единицу по адресу GPIO_BSRR_BR6 
    // (0 - bit reset, 1 - bit set)
    GPIOA -> BSRR |= GPIO_BSRR_BR6;

    // Задержка
    for(i = 0;i < TIME;i++){}

    // Светодиод выключен
    GPIOA -> BSRR |= GPIO_BSRR_BS6;

    // Задержка
    for(i = 0;i < TIME;i++){}

    c++;
    func();
    
  }

  
}

/*************************** End of file ****************************/
