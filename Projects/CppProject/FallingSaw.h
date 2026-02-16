#pragma once

#include "RisingSaw.h"

class FallingSawConfiguration : public SignalGeneratorConfiguration
{
public:
  FallingSawConfiguration(float timesample,
                          float amplitude,
                          float frequency,
                          float dc_shift);  
};//FallingSawConfiguration

class FallingSaw : public SignalGenerator
{
private:
  RisingSaw rising_saw_generator;

public:
  FallingSaw();

public:
  void configure(FallingSawConfiguration& configurator);
  void execute() override;
};//FallingSaw