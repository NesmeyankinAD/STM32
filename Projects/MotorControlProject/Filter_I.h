#pragma once

class FilterIConfiguration
{
public:
  float m_timesample;
  float m_time_constant;

public:
  FilterIConfiguration(float timesample,
                       float time_constant);

};//FilterIConfiguration

class FilterI
{
private:
  float m_timesample;
  float m_time_constant;
  
  float m_reference;
  float m_integral;

public:
  FilterI();

public:
  void  configure(FilterIConfiguration& configurator);
  float execute(float reference);

public:
  void reset();
  void set_output(float firs_condition);

public:
  float get_output();

};//FilterI