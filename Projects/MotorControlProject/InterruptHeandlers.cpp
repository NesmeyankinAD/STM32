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
  //Прерывание 5 кГц

  if (TIM2 -> SR & TIM_SR_CC2IF)   //Произошло совпадение CNT и CCR2 значений
    {
      {//Background operation
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
      
        //---------------------------------------------------------------------//
          adc_handler.convert_data();//Преобразование данных из АЦП
        
          if (currentStrategy != nullptr) {currentStrategy -> execute();}
        //---------------------------------------------------------------------//
      
      TIM2 -> SR &= ~TIM_SR_CC2IF;//Очистка флага прерывания
    }
}

extern "C" void TIM4_IRQHandler()
{
  //Прерывание 20 кГц

  if (TIM4 -> SR & TIM_SR_UIF)
  {
    adc_handler.ADC_start();

    us_counter += 50;//Cчёт по 50 мкс
  }
  
  TIM4 -> SR &= ~TIM_SR_UIF; // Очистка флага прерывания
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

extern "C" void DMA2_Stream0_IRQHandler(void)
{
  //Работа ADC & DMA

  if (DMA2 -> LISR & DMA_LISR_TCIF0)
  {
    DMA2 -> LIFCR = DMA_LIFCR_CTCIF0; //Сброс флага окончания передачи
    
    //adc_handler.ADC_stop();            //Остановка АЦП по событию завершения передачи DMA
    adc_handler.copy_data();           //копирование из буфера DMA в буферы для фаз
  }

  //if (DMA2->LISR & DMA_LISR_HTIF0)      // Half Transfer
  //{
  //  DMA2->LIFCR = DMA_LIFCR_CHTIF0;     // Сброс флага
  //  // Можно добавить счётчик для отладки
  //}
  //
  //if (DMA2->LISR & DMA_LISR_TEIF0)      // Transfer Error ❌
  //{
  //  DMA2->LIFCR = DMA_LIFCR_CTEIF0;     // Сброс флага
  //  // ОШИБКА ПЕРЕДАЧИ!
  //  //while(1); // Остановить для отладки
  //}
  //
  //if (DMA2->LISR & DMA_LISR_FEIF0)      // FIFO Error ❌
  //{
  //  DMA2->LIFCR = DMA_LIFCR_CFEIF0;     // Сброс флага
  //  // ОШИБКА FIFO!
  //  //while(1); // Остановить для отладки
  //}
}