#pragma once

#include "stm32f4xx.h"

#include "SystemInitialization.h"
#include "CoordinateTransformation.h"
#include "PIController.h"
#include "MaxMinModulation.h"
#include "Filter_I.h"
#include "ADCHandler.h"
#include "Observer.h"
#include "SineWave.h"
#include "ThreeSineWave.h"

#define FPWM        float(5000.0)    //Необходимо так же перенастроить счётчики и АЦП на нужные частоты
#define TPWM        float(1.0/FPWM)
#define TIMESAMPLE  TPWM

#define U_DC        float(20.0)

extern int32_t us_counter; //Счётчик микросекунд (TIM4_Handler)

class ControlSystemConfiguration
{
//Объект-конфигуратор САУ
//Должен содержать в себе объекты-конфигураторы всех составных частей САУ
//По ссылке на объект-конфигуратор объекты САУ получают доступ к 
//своим параметрам на этапе конфигурации программы

public:
  PIControllerConfiguration PI_current_loop_configurator;
  PIControllerConfiguration PI_Udc_loop_configurator;

  SineWaveConfiguration      one_phase_sin_generator_config;
  ThreeSineWaveConfiguration three_phase_generator_config;

  PIControllerConfiguration PI_I_d_configurator, PI_I_q_configurator; 
  PIControllerConfiguration PI_w_configurator;           
  PIControllerConfiguration PI_angle_configurator;
  
  IntegratorConfiguration   angle_calc_configurator;       

  ControlSystemConfiguration();

};//ControlSystemConfiguration

class ControlSystem
{
/*//Debug
public:
  float duty_cycle_debug{0.0};  
  float I_ref{0.0};
  float Udc_ref{0.0};
*/

public:
  float angle_ref    {0.0};
  float w_ref        {0.0};
  float I_q_motor_ref{0.0};
  float I_d_motor_ref{0.0};

private:
  float angle_motor{0.0};//Угол поворота двигателя, градусы
  float angle_field{0.0};//Угол поворота поля, градусы
  float w_motor    {0.0};//Скорость вращения двигателя, град/с
  float I_q_motor  {0.0};
  float I_d_motor  {0.0};

public:  
  float U_phases[3]{};//Фазные напряжения при sin модуляции
  float U_maxmin[3]{};//Фазные напряжения при max-min модуляции

  MaxMinModulation modulator;

  float cmp[3]{};

public:
  ADCHandler* adc;                //АЦП
  SynchroMotorObserver* observer; //Наблюдатель двигателя

public:
  ControlSystem(ADCHandler* adc_external, SynchroMotorObserver* observer_external);

/*//Debug
private:
  PIController PI_current_loop;
  PIController PI_Udc_loop;

  SineWave     one_phase_sin_generator;
//Debug*/
  ThreeSineWave three_phase_generator;
  

private:
  PIController PI_I_d, PI_I_q; //Регуляторы тока
  PIController PI_w;           //Регулятор скорости
  PIController PI_angle;       //Регулятор угла поворота

  Integrator angle_calc;

  CoordinateTrasformation coordinate_transform; //Методы преобразования координат

public:
  /*Метод конфигурации САУ
  Должен вызывать методы конфигурации всех составных частей САУ.
  Вызывается на этапе конфигурации программы*/
  void configure(ControlSystemConfiguration& configurator);

public:
  void  speed_calc();
  float continuous_angle(float w);
  void  starter();

public:
  void execute();
  void stop();

};//ControlSystem