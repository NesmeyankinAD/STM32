#include "SineWave.h"


SineWaveConfiguration::
SineWaveConfiguration(float timesample,
                      float amplitude,
                      float frequency,
                      float dc_shift,
                      float phase_shift_grad):
SignalGeneratorConfiguration::
SignalGeneratorConfiguration(timesample, amplitude, frequency, dc_shift)
{
  m_phase_shift_grad = phase_shift_grad;
}

SineWave::SineWave() : SignalGenerator()
{}

void SineWave::configure(SineWaveConfiguration& configurator)
{
  m_timesample = configurator.m_timesample;
  m_amplitude  = configurator.m_amplitude;
  m_frequency  = configurator.m_frequency;
  m_dc_shift   = configurator.m_dc_shift;
  m_phase_shift_grad = configurator.m_phase_shift_grad;

  RisingSawConfiguration rising_saw_generator_config(m_timesample,
                                                     2*M_PI,
                                                     m_frequency,
                                                     0.0);
  rising_saw_generator.configure(rising_saw_generator_config);
}

void SineWave::execute()
{
  static float exec_result = 0.0;

  rising_saw_generator.execute();

  exec_result = m_amplitude * sinf(rising_saw_generator.get_output() + m_phase_shift_grad);

  m_output = exec_result + m_dc_shift;
}