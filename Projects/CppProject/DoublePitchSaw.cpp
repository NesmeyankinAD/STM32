#include "DoublePitchSaw.h"

DoublePitchSawConfiguration::
DoublePitchSawConfiguration(float timesample,
                            float amplitude,
                            float frequency,
                            float dc_shift):
SignalGeneratorConfiguration::
SignalGeneratorConfiguration(timesample, amplitude, frequency, dc_shift)
{}

DoublePitchSaw::DoublePitchSaw() : SignalGenerator()
{}

void DoublePitchSaw::configure(DoublePitchSawConfiguration& configurator)
{
  m_timesample = configurator.m_timesample;
  m_amplitude  = configurator.m_amplitude;
  m_frequency  = configurator.m_frequency;
  m_dc_shift   = configurator.m_dc_shift;

  IntegratorConfiguration integrator_config((1/(m_frequency * m_amplitude)), 
                                            m_timesample, 
                                            m_amplitude, 
                                            -m_amplitude);
  
  MeanderConfiguration    meander_config(m_timesample,
                                         m_amplitude,
                                         m_frequency,
                                         0.0,
                                         50.0);
  
  integrator.configure(integrator_config);
  meander_generator.configure(meander_config);
}

void DoublePitchSaw::execute()
{
  static float exec_result = 0.0;

  meander_generator.execute();
  integrator.execute(meander_generator.get_output());
  
  exec_result = integrator.get_output();

  m_output = exec_result + m_dc_shift;
}