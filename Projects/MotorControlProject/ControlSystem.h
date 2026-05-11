#pragma once

#include "stm32f4xx.h"

#include "SystemInitialization.h"
#include "CoordinateTransformation.h"
#include "PIController.h"
#include "MaxMinModulation.h"
#include "Filter_I.h"
#include "ADCHandler.h"

#include "SineWave.h"

#define FPWM        float(5000.0)    //Необходимо так же перенастроить счётчики и АЦП на нужные частоты
#define TPWM        float(1.0/FPWM)
#define TIMESAMPLE  TPWM

class ControlSystemConfiguration
{
//Объект-конфигуратор САУ
//Должен содержать в себе объекты-конфигураторы всех составных частей САУ
//По ссылке на объект-конфигуратор объекты САУ получают доступ к 
//своим параметрам на этапе конфигурации программы

public:
  PIControllerConfiguration PI_current_loop_configurator;
  PIControllerConfiguration PI_Udc_loop_configurator;

  SineWaveConfiguration     one_phase_sin_generator_config;

  ControlSystemConfiguration();

};//ControlSystemConfiguration

class ControlSystem
{
public:
  float duty_cycle_debug{0.0};  
  float I_ref{0.0};
  float Udc_ref{0.0};

public:
  ADCHandler* adc; 

public:
  ControlSystem(ADCHandler* adc_external);

private:
  PIController PI_current_loop;
  PIController PI_Udc_loop;

  SineWave     one_phase_sin_generator;

public:
  //Метод конфигурации САУ
  //Должен вызывать методы конфигурации всех составных частей САУ.
  //Вызывается на этапе конфигурации программы
  void configure(ControlSystemConfiguration& configurator);

public:
  void execute_current_loop();

public:
  void execute();
  void stop();

};//ControlSystem