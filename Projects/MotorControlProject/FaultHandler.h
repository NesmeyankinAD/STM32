#pragma once

#include "Trigger.h"

enum StateList
{
  NORMALMODE      = 0,
  OVERCURRENT_HW  = 1,
  OVERCURRENT_SW  = 2,
  OVERTEMPERATURE = 3
};//StateList

class FaultHandler
{
private:
  StateList current_state{NORMALMODE};

public:
  Trigger normal_mode;

//Триггеры ошибок: методы set, reset в основном цикле
public:
  Trigger overcurrent_hw;
  Trigger overcurrent_sw;
  Trigger overtemperature;

public:
  void execute();

public:
  StateList get_state();
  bool      get_permission(); 
  
};//FaultHandler