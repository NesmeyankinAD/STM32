#include "General.h"

extern int32_t service_counter;

void ControlStrategy::set_control_system(ControlSystem* cs_ptr)
{
  this -> cs = cs_ptr;
}

StrategyHandler::StrategyHandler(ControlSystem* control_system)
{
  this -> cs = control_system;

  this -> stop.set_control_system(control_system);
  this -> work.set_control_system(control_system);
  this -> standBy.set_control_system(control_system);
}

void StopStrategy::execute()
{
  
}

void WorkStrategy::execute()
{
  service_counter++;

  cs -> execute();
}

void StandByStrategy::execute()
{
  service_counter--;
}

/*

ControlStrategy* StrategyHandler::define_strategy(CurrentStrategy& current_strategy)
{
  switch (current_strategy) 
  {
    case STOP:
    {
      return &stop;
      break;
    }
    case WORK:
    {
      return &work;
      break;
    }
    case STANDBY:
    {
      return &standBy;
      break;
    }
    default:
    {
      return &stop;
    }
  }
}
*/

ControlStrategy* StrategyHandler::define_strategy()
{
  switch (current_strategy) 
  {
    case 0:
    {
      return &stop;
      break;
    }
    case 1:
    {
      return &work;
      break;
    }
    case 2:
    {
      return &standBy;
      break;
    }
    default:
    {
      return &stop;
    }
  }
}

  void StrategyHandler::set_stop()
  {
    current_strategy = STOP;
  }

  void StrategyHandler::set_work()
  {
    current_strategy = WORK;
  }

  void StrategyHandler::set_standBy()
  {
    current_strategy = STANDBY;
  }  