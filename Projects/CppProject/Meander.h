#pragma once

#include "SignalGenerator.h"


class MeanderConfiguration : public SignalGeneratorConfiguration
{
public:
  float m_duty_cycle;

public:
  MeanderConfiguration(float timesample,
                       float amplitude,
                       float frequency,
                       float dc_shift,
                       float duty_cycle);

};//MeanderConfiguration


class Meander : public SignalGenerator
{
private:
  float m_duty_cycle;
  int   iterator;
  int   n, n1, n2;

public:
  Meander();
  
public:
  void configure(MeanderConfiguration& configurator);
  void execute() override;
};//Meander

