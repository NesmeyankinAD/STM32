#pragma once

#include "SignalGenerator.h"
#include "Meander.h"
#include "Integrator.h"

class DoublePitchSawConfiguration : public SignalGeneratorConfiguration
{
public:
  DoublePitchSawConfiguration(float timesample,
                              float amplitude,
                              float frequency,
                              float dc_shift);
};//DoublePitchSawConfiguration


class DoublePitchSaw : public SignalGenerator
{
private:
  Meander    meander_generator;
  Integrator integrator;

public:
  DoublePitchSaw();
  
public:
  void configure(DoublePitchSawConfiguration& configurator);
  void execute() override;   
};//DoublePitchSaw