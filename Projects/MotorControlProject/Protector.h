#pragma once

#include "Trigger.h"

class ProtectorConfiguration
{
  
};//ProtectorConfiguration

class Protector
{
private:
  Trigger instant_current_trigger;
  Trigger average_current_trigger;

public:
  Protector();

public:
  void configure();
  void execute();


};//Protector