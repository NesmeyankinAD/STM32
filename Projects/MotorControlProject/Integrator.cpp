#include "Integrator.h"
#include <math.h>

IntegratorConfiguration::
IntegratorConfiguration(float time_constant,
                        float timesample,
                        float high_saturation,
                        float low_saturation)
{
  m_timesample      = timesample;
  m_time_constant   = time_constant;
  m_high_saturation = high_saturation;
  m_low_saturation  = low_saturation;
}

Integrator::Integrator():
  m_timesample      (0.0),
  m_input           (0.0),
  m_time_constant   (0.0),
  m_high_saturation (0.0),
  m_low_saturation  (0.0),
  m_exec_result     (0.0),
  m_output          (0.0)
{}

void Integrator::configure(IntegratorConfiguration& configurator)
{
  m_timesample      = configurator.m_timesample;
  m_time_constant   = configurator.m_time_constant;
  m_high_saturation = configurator.m_high_saturation;
  m_low_saturation  = configurator.m_low_saturation;
}

void Integrator::execute(float input)
{
  static float exec_result = 0.0;
  
  m_input = input;
  
  exec_result   = (m_timesample/m_time_constant) * m_input + m_output;
  m_exec_result = exec_result;
  
  m_output = fminf(exec_result, m_high_saturation);
  m_output = fmaxf(m_output, m_low_saturation);
}

float Integrator::get_output()
{
  return m_output;
}

void Integrator::set_initial_output(float initial_condition)
{
  m_output = initial_condition;
}

void Integrator::reset()
{
  m_output      = 0.0;
  m_exec_result = 0.0;
}