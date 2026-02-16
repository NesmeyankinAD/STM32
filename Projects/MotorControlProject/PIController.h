#pragma once

#include <math.h>
#include <stdint.h>

class PIControllerConfiguration
{
public:
  float m_timesample;
  float m_gain;
  float m_time_constant;
  float m_high_saturation;
  float m_low_saturation;

  PIControllerConfiguration(float timesample,
                            float gain,
                            float time_constant,
                            float high_saturation,
                            float low_saturation);

};//PIControllerConfiguration


class PIController
{
private:
  float m_timesample;
  float m_gain;
  float m_time_constant;
  float m_high_saturation;
  float m_low_saturation;

private:
  float m_reference;
  float m_feedback;
  float m_delta;
  float m_integral;
  float m_summ;
  float m_output;

public:
  PIController();

public:
  void  configure(PIControllerConfiguration& configurator);
  float execute(float reference, float feedback);

public:
  void reset();
  void set_output(float first_condition);

public:
  float get_output();
};//PIController