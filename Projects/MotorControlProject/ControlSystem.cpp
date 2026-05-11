#include "ControlSystem.h"

ControlSystem::ControlSystem(ADCHandler* adc_external)
{
  adc = adc_external;
}

void ControlSystem::configure(ControlSystemConfiguration& configurator)
{
  PI_current_loop.configure(configurator.PI_current_loop_configurator);
  PI_Udc_loop.configure(configurator.PI_Udc_loop_configurator);
  one_phase_sin_generator.configure(configurator.one_phase_sin_generator_config);
}

void ControlSystem::execute_current_loop()
{
  TIM1_Start();

  
}

void ControlSystem::execute()
{
  TIM1_Start();

  GPIOC -> BSRR &= ~(1 << 20); //PWM Enable - 0
  
  //
  //PI_Udc_loop.execute(Udc_ref, adc -> ADC_data_converted[3]);
  //
  //PI_current_loop.execute(PI_Udc_loop.get_output(), adc -> ADC_data_converted[0]);
  //
  //TIM1 -> CCR1 = (uint32_t)((PI_current_loop.get_output()/100.0) * 16800.0);
  //

  //
  one_phase_sin_generator.execute();

  float cmp_direct  = (0.5 * (one_phase_sin_generator.get_output()/20.0 + 1) * 16800.0);

  float cmp_inverse = (16800.0 - (0.5 * (one_phase_sin_generator.get_output()/20.0 + 1) * 16800.0));

  TIM1 -> CCR1 = (uint32_t)cmp_direct;
  TIM1 -> CCR2 = (uint32_t)cmp_inverse;

  //

  //TIM1 -> CCR1 = (uint32_t)((duty_cycle_debug/100.0) * 16800.0);
}

void ControlSystem::stop()
{
  TIM1 -> CCR1 = 0;
  TIM1 -> CCR2 = 0;
  TIM1 -> CCR3 = 0;
  TIM1_Stop();

  GPIOE -> BSRR &= ~(0x3F << 24); //~(0011 1111 0000 0000 0000 0000 0000 0000)
                                  //Ручное обнуление ШИМ-пинов PE8-PE13
}