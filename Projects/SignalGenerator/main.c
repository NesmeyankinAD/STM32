#include "stm32f4xx.h"

#include "Integrator.h"

/*
Запуск синхронного счёта TIM1:Slave mode - reset mode и TIM2:Master mode - Update.
По событию обновления TIM1 синхронизируется с TIM2.

По результату конвертации АЦП1:IN2(PA2) изменяется яркость свечения светодиода D3(PA7).
*/

//#define
#define LED_D2_ON  GPIOA -> BSRR |= GPIO_BSRR_BR6;
#define LED_D2_OFF GPIOA -> BSRR |= GPIO_BSRR_BS6;

//Глобальные переменные
uint16_t ms_counter = 0; //Счётчик милисекунд
uint16_t led_counter = 0;
uint16_t duty = 0;

//Объявление функций
void RCC_Init(void);
void GPIO_Init(void);
void TIM1_Init(void);
void TIM2_Init(void);
void TIM_Start(void);
void ADC1_Init(void);
void ADC_IRQHandler(void);

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

}

void TIM2_IRQHandler(void)
{
  if (TIM2 -> SR & TIM_SR_CC1IF)   //Произошло совпадение CNT и CCR2 значений
    {
        led_counter++;

        if(led_counter == 1000)
        {
          if(((GPIOA -> ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6)) {LED_D2_ON}
          else                                                {LED_D2_OFF}

          led_counter = 0;
        }
        
      

        TIM2 -> SR &= ~TIM_SR_CC1IF; // Очистка флага прерывания
    }
}

void ADC_IRQHandler(void)
{
  /*В регистр сравнения TIM1:CCR1 записываем результат преобразования АЦП1, 
  привёденный к значению регистра ARR = 28000 -1. Регистр TIM1:CCR1 и ADC1:DR 16-битные.
                               TIMx:ARR
  TIMx -> CCRy = ADCz -> DR --------------
                             bit_rate_ADCz   
  */

  TIM1 -> CCR1 = (ADC1 -> DR * 28000) / 4096;
  
  duty =  TIM1 -> CCR1;

  NVIC_ClearPendingIRQ(ADC_IRQn); //Сброс флага запроса прерывания
}


int main(void)
{
  SystemInit();
  RCC_Init();
  GPIO_Init();
  TIM1_Init();
  TIM2_Init();   
  ADC1_Init();


  SysTick_Config(168000);//Прерывание по SysTick каждые 1мс.

  TIM_Start();           //Запуск таймеров TIM1 и TIM2

  IntegratorConfiguration configurator(2.0, )

  while(1)
  {


  }
}