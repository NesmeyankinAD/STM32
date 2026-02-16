#pragma once

#include <stdint.h>
#include "ControlSystem.h"

class ControlStrategy
{//Родительский класс для стратегий-режимов работы

protected:
  ControlSystem* cs = nullptr;                    //Указатель на САУ

public:
  void set_control_system(ControlSystem* cs_ptr); //Метод установки указателя на САУ извне

public:
  virtual void execute() = 0;
};//ControlStrategy


class StopStrategy : public ControlStrategy
{
public:
  void execute() override;

};//StopStrategy 

class WorkStrategy : public ControlStrategy
{
public:
  void execute() override;

};//WorkStrategy

class StandByStrategy : public ControlStrategy
{
public:
  void execute() override;

};//StandByStrategy


enum CurrentStrategy
{
  STOP    = 0,
  WORK    = 1,
  STANDBY = 2
};

class StrategyHandler
{
public:
  CurrentStrategy current_strategy = STOP;

public:
  ControlSystem* cs{nullptr};                     //Указатель на САУ

private:
  StopStrategy     stop;                          //Режимы работы - стратегии
  WorkStrategy     work;
  StandByStrategy  standBy;
  
public:
  StrategyHandler(ControlSystem* control_system); //Конструктор

public:
  ControlStrategy* define_strategy();             //Метод определения режима работы в while(1)
  
public:
  void set_stop();                                //Установка режима работы - стратегии
  void set_work();
  void set_standBy();   
};//Strategy

