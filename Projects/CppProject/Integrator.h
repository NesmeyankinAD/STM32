#pragma once

volatile class IntegratorConfiguration
{
public:
  float m_time_constant;
  float m_timesample;
  float m_high_saturation;
  float m_low_saturation;

public:
  IntegratorConfiguration (float time_constant,
                           float timesample,
                           float high_saturation,
                           float low_saturation);
  
  //~IntegratorConfiguration(); //Удаление конфигуратора после конфигурации
};//IntegratorConfiguration

volatile class Integrator
{
private:
  float m_time_constant;
  float m_timesample;
  float m_high_saturation;
  float m_low_saturation;

private:
  float m_exec_result;
  float m_output;
  float m_input;

public:
  Integrator();

public:
  void  configure(IntegratorConfiguration& configurator);
  void  execute(float input);
  void  reset();
  void  set_initial_output(float initial_condition);
  float get_output();
};//Integrator