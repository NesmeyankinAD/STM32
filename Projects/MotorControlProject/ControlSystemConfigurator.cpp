#include "ControlSystem.h"

//Здесь параметры объектов - составных частей САУ, задаются через конструкторы соответствующих
//объектов-конфигураторов

ControlSystemConfiguration::ControlSystemConfiguration():
  PI_current_loop_configurator(1.0,1.0,1.0,1.0,1.0)
{}
