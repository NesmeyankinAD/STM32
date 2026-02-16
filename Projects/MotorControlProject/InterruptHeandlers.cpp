#include "InterruptHeandlers.h"

extern "C" void SysTick_Handler()
{
/*Прерывание по SysTick таймеру (24-bit downcounter).
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

extern "C" void TIM2_IRQHandler()
{
  if (TIM2 -> SR & TIM_SR_CC2IF)   //Произошло совпадение CNT и CCR2 значений
    {
        {//background operation
        led_counter++;

        if(led_counter == 5000)
        {
          if(((GPIOA -> ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6)) {LED_D2_ON}
          else                                                {LED_D2_OFF}

          led_counter = 0;
        }

        {//one-phase generator execute
        //one_phase_sin_generator.execute();

        //DAC -> DHR12R1 = (one_phase_sin_generator.get_output() / 10) * 4095; //DAC1_OUT - GPIOA4
        }

        {//three-phase generator execute
        //three_phase_sin_generator.execute();

        //DAC -> DHR12R1 = (three_phase_sin_generator.get_output_A() / 10) * 4095; //DAC1_OUT - GPIOA4
        //DAC -> DHR12R2 = (three_phase_sin_generator.get_output_C() / 10) * 4095; //DAC2_OUT - GPIOA5
        }
        }

        
        if (currentStrategy != nullptr) {currentStrategy -> execute();}

        TIM2 -> SR &= ~TIM_SR_CC2IF; // Очистка флага прерывания
    }
}

extern "C" void TIM4_IRQHandler()
{
  if (TIM4 -> SR & TIM_SR_UIF)
  {
    us_counter++;
  }
  
  TIM4 -> SR &= ~TIM_SR_UIF; // Очистка флага прерывания
}

extern "C" void ADC_IRQHandler()
{
  /*В регистр сравнения TIM1:CCR1 записываем результат преобразования АЦП1, 
  привёденный к значению регистра ARR = 28000 -1. Регистр TIM1:CCR1 и ADC1:DR 16-битные.
                               TIMx:ARR
  TIMx -> CCRy = ADCz -> DR --------------
                             bit_rate_ADCz   
  */

  TIM1 -> CCR1 = (ADC1 -> DR * 28000) / 4096;
  
  //duty =  TIM1 -> CCR1;

  NVIC_ClearPendingIRQ(ADC_IRQn); //Сброс флага запроса прерывания
}

extern "C" void EXTI1_IRQHandler(void)
{
EXTI -> PR |= EXTI_PR_PR1;  //Сброс бита запроса прерывания

//--------Программа обработчика прерывания----------------------//

observer.set_present_time(us_counter);

observer.compute_rotation_direction();

observer.compute_angle();

observer.compute_rotation_frequency();

observer.set_previous_time(us_counter);

//--------Программа обработчика прерывания----------------------//

}

extern "C" void EXTI2_IRQHandler(void)
{
EXTI -> PR |= EXTI_PR_PR2;  // Сброс бита запроса прерывания

//--------Программа обработчика прерывания----------------------//

observer.set_present_time(us_counter);

observer.compute_rotation_direction();

observer.compute_angle();

observer.compute_rotation_frequency();

observer.set_previous_time(us_counter);

//--------Программа обработчика прерывания----------------------//

}

extern "C" void EXTI3_IRQHandler(void)
{
EXTI -> PR |= EXTI_PR_PR3;  //Сброс бита запроса прерывания

//--------Программа обработчика прерывания----------------------//

observer.set_present_time(us_counter);

observer.compute_rotation_direction();

observer.compute_angle();

observer.compute_rotation_frequency();

observer.set_previous_time(us_counter);

//--------Программа обработчика прерывания----------------------//

}