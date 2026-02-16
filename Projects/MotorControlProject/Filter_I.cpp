#include "Filter_I.h"

FilterIConfiguration::
FilterIConfiguration(float timesample,
                     float time_constant)
{
  m_timesample    = timesample;
  m_time_constant = time_constant;
}

FilterI::FilterI():
  m_timesample    (0.0),
  m_time_constant (0.0),
  m_integral      (0.0),
  m_reference     (0.0)
{}

void FilterI::configure(FilterIConfiguration& configurator)
{
  m_timesample    = configurator.m_timesample;
  m_time_constant = configurator.m_time_constant;
}

float FilterI::execute(float reference)
{
    m_reference = reference;
    return m_integral += (m_timesample / m_time_constant) * (m_reference - m_integral);
}

void FilterI::reset()
{
    m_reference = 0.0;
    m_integral  = 0.0;
}

void FilterI::set_output(float first_condition)
{
  m_integral = first_condition;
}

float FilterI::get_output()
{
  return m_integral;
}