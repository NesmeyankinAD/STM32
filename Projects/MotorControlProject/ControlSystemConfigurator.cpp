#include "ControlSystem.h"

//Здесь параметры объектов - составных частей САУ, задаются через конструкторы соответствующих
//объектов-конфигураторов

ControlSystemConfiguration::ControlSystemConfiguration():
  PI_current_loop_configurator  (TIMESAMPLE, 5, 0.003, 100.0, 0.0),
  PI_Udc_loop_configurator      (TIMESAMPLE, 0.5, 0.01, 4.0, 0.0),
  one_phase_sin_generator_config(TIMESAMPLE, 10.0, 50.0, 0.0, 0.0),
  three_phase_generator_config  (TIMESAMPLE, 1.5, 10.0, 0.0, 0.0),

  PI_I_d_configurator  (TIMESAMPLE, 0.2, 0.01, 0.9*U_DC, -0.9*U_DC),
  PI_I_q_configurator  (TIMESAMPLE, 0.2, 0.01, 0.9*U_DC, -0.9*U_DC),
  PI_w_configurator    (TIMESAMPLE, 0.005, 0.06, 3.0, -3.0),
  PI_angle_configurator(TIMESAMPLE, 1, 0.08, 150.0, -150.0),
  angle_calc_configurator(1.0, TIMESAMPLE, 100000000.0, -100000000.0)
{}
