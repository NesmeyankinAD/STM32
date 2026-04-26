#pragma once

#include "stm32f4xx.h"

#include "SystemInitialization.h"
#include "CoordinateTransformation.h"
#include "PIController.h"
#include "MaxMinModulation.h"
#include "Filter_I.h"
#include "ADCHandler.h"

#define FPWM       5000.0
#define TPWM       1/FPWM
#define TIMESAMPLE TPWM

class ControlSystemConfiguration
{
//Объект-конфигуратор САУ
//Должен содержать в себе объекты-конфигураторы всех составных частей САУ
//По ссылке на объект-конфигуратор объекты САУ получают доступ к 
//своим параметрам на этапе конфигурации программы

public:
  PIControllerConfiguration PI_current_loop_configurator;

  ControlSystemConfiguration();

};//ControlSystemConfiguration

class ControlSystem
{
public:
  float duty_cycle_debug{0.0};  
  float I_ref{0.0};

public:
  ADCHandler* adc; 

public:
  ControlSystem(ADCHandler* adc_external);

private:
  PIController PI_current_loop;

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