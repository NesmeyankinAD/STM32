#pragma once

#include "SineWave.h"

class ThreeSineWaveConfiguration : public SignalGeneratorConfiguration
{
public:
  float m_phase_shift_grad;

public:
  ThreeSineWaveConfiguration(float timesample,
                             float amplitude,
                             float frequency,
                             float dc_shift,
                             float phase_shift_grad);
};//ThreeSineWaveConfiguration


class ThreeSineWave : public SignalGenerator
{
private:
  float m_phase_shift_grad;
  float m_output_A;
  float m_output_B;
  float m_output_C;

private:
  SineWave phase_A, phase_B, phase_C;
  
public:
  ThreeSineWave();
  
public:
  void  configure(ThreeSineWaveConfiguration& configurator);
  void  execute() override;
  void  reset();    
  float get_output_A();
  float get_output_B(); 
  float get_output_C();
};//ThreeSineWave