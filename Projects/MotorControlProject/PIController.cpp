#include "PIController.h"

PIControllerConfiguration::
PIControllerConfiguration(float timesample,
                          float gain,
                          float time_constant,
                          float high_saturation,
                          float low_saturation)
{
  m_timesample      = timesample;
  m_gain            = gain;
  m_time_constant   = time_constant;
  m_high_saturation = high_saturation;
  m_low_saturation  = low_saturation; 
}

PIController::PIController():
  m_timesample      (0.0),
  m_gain            (0.0),
  m_time_constant   (0.0),
  m_high_saturation (0.0),
  m_low_saturation  (0.0),
  m_reference       (0.0),
  m_feedback        (0.0),
  m_delta           (0.0),
  m_integral        (0.0),
  m_summ            (0.0),
  m_output          (0.0)
{}

void PIController::configure(PIControllerConfiguration& configurator)
{
  m_timesample      = configurator.m_timesample;
  m_gain            = configurator.m_gain;
  m_time_constant   = configurator.m_time_constant;
  m_high_saturation = configurator.m_high_saturation;
  m_low_saturation  = configurator.m_low_saturation;
}

float PIController::execute(float reference, float feedback)
{
  static float _out = 0.0;
  
  m_reference = reference;
  m_feedback  = feedback;
  
  m_delta     = m_gain * (m_reference - m_feedback);
  m_integral += (m_timesample / m_time_constant) * (m_delta - m_summ + m_output);
  m_summ      = m_integral + m_delta;
  
  _out = fminf(m_summ, m_high_saturation);
  _out = fmaxf(_out, m_low_saturation);
  
  return m_output = _out;
}

  void PIController::reset()
  {
    m_reference = 0.0;
    m_feedback  = 0.0;
    m_delta     = 0.0;
    m_integral  = 0.0;
    m_summ      = 0.0;
    m_output    = 0.0;
  }

  void PIController::set_output(float first_condition)
  {
    m_integral = fminf(first_condition, m_high_saturation);
    m_integral = fmaxf(m_integral, m_low_saturation);
    m_summ = m_integral;
    m_output = m_integral;
  }

  float PIController::get_output()
  {
    return m_output;
  }