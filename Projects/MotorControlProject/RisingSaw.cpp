#include "RisingSaw.h"

RisingSawConfiguration::
RisingSawConfiguration(float timesample,
                       float amplitude,
                       float frequency,
                       float dc_shift):
SignalGeneratorConfiguration::
SignalGeneratorConfiguration(timesample, amplitude, frequency, dc_shift)
{}

RisingSaw::RisingSaw() : SignalGenerator()
{}

void RisingSaw::configure(RisingSawConfiguration& configurator)
{  
  m_timesample = configurator.m_timesample;
  m_amplitude  = configurator.m_amplitude;
  m_frequency  = configurator.m_frequency;
  m_dc_shift   = configurator.m_dc_shift;
  
  IntegratorConfiguration Integrator_config((1/(m_frequency * m_amplitude)), 
                                            m_timesample, 
                                            m_amplitude, 
                                            -m_amplitude);
  integrator.configure(Integrator_config);
}

void RisingSaw::execute()
{
  static float exec_result = 0.0;

  int n = 1 / (m_frequency * m_timesample);

  if(n < 20) {exec_result = 0.0;}
  else
  {
    integrator.execute(1);
    exec_result = integrator.get_output();
  }

  if(exec_result >= m_amplitude) 
  {
    integrator.reset();
    exec_result = 0.0;
  }

  m_output = exec_result + m_dc_shift;
}