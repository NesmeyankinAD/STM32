#include "ControlSystem.h"

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