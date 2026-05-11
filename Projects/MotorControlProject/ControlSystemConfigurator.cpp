#include "ControlSystem.h"

//Здесь параметры объектов - составных частей САУ, задаются через конструкторы соответствующих
//объектов-конфигураторов

ControlSystemConfiguration::ControlSystemConfiguration():
  PI_current_loop_configurator  (TIMESAMPLE, 5, 0.003, 100.0, 0.0),
  PI_Udc_loop_configurator      (TIMESAMPLE, 0.5, 0.01, 4.0, 0.0),
  one_phase_sin_generator_config(TIMESAMPLE, 10.0, 50.0, 0.0, 0.0)
{}
