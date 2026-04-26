#include "ControlSystem.h"

ControlSystem::ControlSystem(ADCHandler* adc_external)
{
  adc = adc_external;
}

void ControlSystem::configure(ControlSystemConfiguration& configurator)
{
  PI_current_loop.configure(configurator.PI_current_loop_configurator);
}

void ControlSystem::execute_current_loop()
{
  TIM1_Start();

  
}

void ControlSystem::execute()
{
  TIM1_Start();

  GPIOC -> BSRR &= ~(1 << 20); //PWM Enable - 0
  
  PI_current_loop.execute(I_ref, adc -> ADC_data_converted[0]);

  TIM1 -> CCR1 = (uint32_t)((PI_current_loop.get_output()/100.0) * 16800.0);

  //TIM1 -> CCR1 = (uint32_t)((duty_cycle_debug/100.0) * 16800.0);
}

void ControlSystem::stop()
{
  TIM1_Stop();

  GPIOE -> BSRR &= ~(0x3F << 24); //~(0011 1111 0000 0000 0000 0000 0000 0000)
                                  //Ручное обнуление ШИМ-пинов PE8-PE13
}