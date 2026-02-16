#include "stm32f4xx.h"

void GPIO_Init(void)
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