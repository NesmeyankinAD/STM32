#pragma once

#include "stm32f4xx.h"

#include "SystemInitialization.h"
#include "CoordinateTransformation.h"
#include "PIController.h"
#include "MaxMinModulation.h"
#include "Filter_I.h"

class ControlSystemConfiguration
{
  
};//ControlSystemConfiguration

class ControlSystem
{
public:
  void execute();
  void stop();

};//ControlSystem