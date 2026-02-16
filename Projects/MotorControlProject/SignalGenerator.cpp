#include "SignalGenerator.h"

SignalGeneratorConfiguration::
SignalGeneratorConfiguration(float timesample,
                             float amplitude,
                             float frequency,
                             float dc_shift)
{
  m_timesample = timesample;
  m_amplitude  = amplitude;
  m_frequency  = frequency;
  m_dc_shift   = dc_shift;
}

SignalGenerator::SignalGenerator():
  m_output     (0.0),
  m_amplitude  (0.0),
  m_dc_shift   (0.0),
  m_frequency  (0.0),
  m_timesample (0.0)
{}

void SignalGenerator::configure(SignalGeneratorConfiguration& configurator)
{
  m_amplitude  = configurator.m_amplitude;
  m_dc_shift   = configurator.m_dc_shift;
  m_frequency  = configurator.m_frequency;
  m_timesample = configurator.m_timesample;
}

void SignalGenerator::execute()
{
  static float exec_result;

  exec_result += 1;
  m_output = exec_result;
}

float SignalGenerator::get_output()
{
  return m_output;
}

void SignalGenerator::reset()
{
  m_output = 0.0;
}
