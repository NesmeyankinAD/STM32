#include "ControlSystem.h"

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

}

void ControlSystem::stop()
{
  TIM1_Stop();

  GPIOE -> BSRR &= ~(0x3F << 24); //~(0011 1111 0000 0000 0000 0000 0000 0000)
                                  //Ручное обнуление ШИМ-пинов PE8-PE13
}