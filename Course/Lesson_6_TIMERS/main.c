#include "stm32f4xx.h"

/*
Мигание по прерыванию SysTick светидиодом D2 (GPIOA_6)

Плавное мигание светодиодом D3 (GPIOA_7) с помощью 
канала CH1N (комплиментарный CH1) счётчика TIM8.
*/

//#define
#define LED_D2_ON  GPIOA -> BSRR |= GPIO_BSRR_BR6;
#define LED_D2_OFF GPIOA -> BSRR |= GPIO_BSRR_BS6;

//Глобальные переменные
uint16_t ms_counter = 0; //Счётчик милисекунд
uint16_t delay_500  = 0;
uint16_t delay_10   = 0;

uint16_t duty = 0;


//Объявление функций
void RCC_Init(void);
void GPIO_Init(void);
void TIM8_Init(void);

//Прерывания
void SysTick_Handler(void)
{/*Прерывание по SysTick таймеру (24-bit downcounter).
Частота срабатывания прерывания настраивается вызовом SysTlick_Config(x);
            AHB_FREQ
где x = ------------------
        TIME_INTERRUPT_REF

TIME_INTERRUPT_REF = 0.001s
AHB_FREQ = 168 MHz
x = 168000
*/    
  ms_counter++;

  //if(((GPIOA -> ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) && (ms_counter%1000 == 0)) {LED_D2_ON}
  //else                                                                          {LED_D2_OFF}
}


int main(void)
{
  SystemInit();
  RCC_Init();
  GPIO_Init();
  TIM8_Init();  

  SysTick_Config(168000); //Прерывание по SysTick каждые 1мс.

  
  while(1)
  {
  
    //if(delay_500 < ms_counter)
    //{
    //  if((GPIOA -> ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) {LED_D2_ON}
    //  else                                              {LED_D2_OFF}
    //
    //  delay_500 = ms_counter + 1000;
    //}
  

  /*---Плавное мигание светодиодом D3 (GPIOA_7)---*/
  
    //if(delay_10 <= ms_counter)
    //{
    //  if(TIM8 -> CCR1 >= (1000-1)) {TIM8 -> CCR1 = 0;}
    //  else                       {TIM8 -> CCR1 = TIM8 -> CCR1 + 5;}
    //  
    //  delay_10 = ms_counter + 10;
    //}

  TIM8 -> CCR1 = duty;

  }
}