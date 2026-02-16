#pragma once

#include "SignalGenerator.h"
#include "Integrator.h"

class RisingSawConfiguration : public SignalGeneratorConfiguration
{
public:
  RisingSawConfiguration(float timesample,
                         float amplitude,
                         float frequency,
                         float dc_shift);

};//RisingSawConfiguration


class RisingSaw : public SignalGenerator
{
private:
  Integrator integrator;

public:
  RisingSaw();

public:
  void configure(RisingSawConfiguration& configurator);
  void execute() override;
};//RisingSaw