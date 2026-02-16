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
  if (TIM2 -> SR & TIM_SR_CC1IF)   //Произошло совпадение CNT и CCR2 значений
    {
        led_counter++;

        if(led_counter == 1000)
        {
          if(((GPIOA -> ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6)) {LED_D2_ON}
          else                                                {LED_D2_OFF}

          led_counter = 0;
        }
        
        ms_counter++;

        {//integrator execute
        //integrator_1.execute(1);
        //
        //integratorScope = integrator_1.get_output();
        //  if(integratorScope == 5.0) integrator_1.reset();
        //
        //TIM1 -> CCR1 = (integratorScope / 5) * 28000;
        //
        //DAC -> DHR12R1 = (integratorScope / 5) * 4095; //DAC1_OUT - GPIOA4
        }

        {//meander execute
        meander_generator.execute();
        
        //DAC -> DHR12R1 = (meander_generator.get_output() / 10) * 4095; //DAC1_OUT - GPIOA4
        }

        {//rising saw execute
        rising_saw_generator.execute();
          
        //DAC -> DHR12R1 = (rising_saw_generator.get_output() / 10) * 4095; //DAC1_OUT - GPIOA4
        }

        {//falling saw execute
        falling_saw_generator.execute();

        //DAC -> DHR12R1 = (falling_saw_generator.get_output() / 10) * 4095; //DAC1_OUT - GPIOA4
        }

        {//double-pitch saw execute
        double_pitch_saw_generator.execute();

        DAC -> DHR12R1 = (double_pitch_saw_generator.get_output() / 10) * 4095; //DAC1_OUT - GPIOA4
        
        TIM1 -> CCR1 = (double_pitch_saw_generator.get_output() / 5) * 28000;
        }

        {//one-phase generator execute
        one_phase_sin_generator.execute();

        //DAC -> DHR12R1 = (one_phase_sin_generator.get_output() / 10) * 4095; //DAC1_OUT - GPIOA4
        }

        {//three-phase generator execute
        three_phase_sin_generator.execute();

        //DAC -> DHR12R1 = (three_phase_sin_generator.get_output_A() / 10) * 4095; //DAC1_OUT - GPIOA4
        //DAC -> DHR12R2 = (three_phase_sin_generator.get_output_C() / 10) * 4095; //DAC2_OUT - GPIOA5
        }

        TIM2 -> SR &= ~TIM_SR_CC1IF; // Очистка флага прерывания
    }
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
  
  duty =  TIM1 -> CCR1;

  NVIC_ClearPendingIRQ(ADC_IRQn); //Сброс флага запроса прерывания
}