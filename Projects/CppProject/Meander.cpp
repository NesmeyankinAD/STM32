#include "Meander.h"


MeanderConfiguration::
MeanderConfiguration(float timesample,
                     float amplitude,
                     float frequency,
                     float dc_shift,
                     float duty_cycle): 
SignalGeneratorConfiguration::
SignalGeneratorConfiguration(timesample, amplitude, frequency, dc_shift)
{
  m_duty_cycle = duty_cycle;
}


Meander::Meander() : SignalGenerator()
{
  m_duty_cycle = 0.0;
  iterator     = 0;
  n            = 0;
  n1           = 0;
  n2           = 0;
}

void Meander::configure(MeanderConfiguration& configurator)
{
  m_amplitude  = configurator.m_amplitude;
  m_dc_shift   = configurator.m_dc_shift;
  m_frequency  = configurator.m_frequency;
  m_timesample = configurator.m_timesample;
  m_duty_cycle = configurator.m_duty_cycle;

  n   = 1 / (m_frequency * m_timesample);
  n1  = (m_duty_cycle / 100) * n;
  n2  = n - n1;
}

void Meander::execute()
{
  static float exec_result = 0.0;

  if(n < 2)                            {exec_result = 0.0;}
  else if((n >= 2) && (iterator < n1)) {exec_result = m_amplitude;}
  else                                 {exec_result = -m_amplitude;}

  m_output = exec_result + m_dc_shift;

  iterator++;
  if(iterator == n) iterator = 0;
} 

