#include "FallingSaw.h"

FallingSawConfiguration::
FallingSawConfiguration(float timesample,
                        float amplitude,
                        float frequency,
                        float dc_shift):
SignalGeneratorConfiguration::
SignalGeneratorConfiguration(timesample, amplitude, frequency, dc_shift)
{}

FallingSaw::FallingSaw() : SignalGenerator()
{}

void FallingSaw::configure(FallingSawConfiguration& configurator)
{
  m_timesample = configurator.m_timesample;
  m_amplitude  = configurator.m_amplitude;
  m_frequency  = configurator.m_frequency;
  m_dc_shift   = configurator.m_dc_shift;

  RisingSawConfiguration rising_saw_generator_config(m_timesample,
                                                     m_amplitude,
                                                     m_frequency,
                                                     0.0);

  rising_saw_generator.configure(rising_saw_generator_config);
}

void FallingSaw::execute()
{
  static float exec_result = 0.0;

  rising_saw_generator.execute();

  exec_result = m_amplitude - rising_saw_generator.get_output();
  
  m_output = exec_result + m_dc_shift;
}