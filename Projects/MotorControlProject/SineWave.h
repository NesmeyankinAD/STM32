#pragma once

#include "RisingSaw.h"
#include <math.h>

class SineWaveConfiguration : public SignalGeneratorConfiguration
{
public:
  float m_phase_shift_grad;

public:
  SineWaveConfiguration(float timesample,
                        float amplitude,
                        float frequency,
                        float dc_shift,
                        float phase_shift_grad);
};//SineWaveConfiguration


class SineWave : public SignalGenerator
{
private:
  float m_phase_shift_grad;

private:
  RisingSaw rising_saw_generator;
  
public:
  SineWave();
  
public:
  void configure(SineWaveConfiguration& configurator);
  void execute() override;     
};//SineWave