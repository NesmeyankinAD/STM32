
#include "InterruptHeandlers.h"

void SysTick_Handler()
{/*Прерывание по SysTick таймеру (24-bit downcounter).
Частота срабатывания прерывания настраивается вызовом SysTlick_Config(x);
            AHB_FREQ
где x = ------------------
        TIME_INTERRUPT_REF

TIME_INTERRUPT_REF = 0.001s
AHB_FREQ = 168 MHz
x = 168000
*/    
  ms_counter++; //Milliseconds

}

void TIM2_IRQHandler()
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
        
        integrator_1.execute(1);

        integratorScope = integrator_1.get_output();

        TIM2 -> SR &= ~TIM_SR_CC1IF; // Очистка флага прерывания
    }
}

void ADC_IRQHandler()
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