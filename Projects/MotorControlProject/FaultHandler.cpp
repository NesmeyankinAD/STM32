#include "FaultHandler.h"

void FaultHandler::execute()
{
  if(overcurrent_hw.get_quit())
  {
    current_state = OVERCURRENT_HW;
    normal_mode.reset(true);
  }
  else 
  if(overcurrent_sw.get_quit())
  {
    current_state = OVERCURRENT_SW;
    normal_mode.reset(true);
  }
  else
  if(overtemperature.get_quit())
  {
    current_state = OVERTEMPERATURE;
    normal_mode.reset(true);
  }
  else 
  {
    current_state = NORMALMODE;
    normal_mode.set(true);
  }
}

StateList FaultHandler::get_state()
{
  return current_state;
}

bool FaultHandler::get_permission()
{
  if(normal_mode.get_quit()) return true;
  else                       return false;
}