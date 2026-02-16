#include "Trigger.h"

Trigger::Trigger():
  quit(false),
  previous_state(false)
{}

void Trigger::set(bool expression)
{
  if(expression) 
  {
    previous_state = quit;
    quit = true;
  }   
}

void Trigger::reset(bool expression)
{
  if(expression) 
  {
    previous_state = quit;
    quit = false;
  }   
}

bool Trigger::get_quit()
{
  return quit;
}