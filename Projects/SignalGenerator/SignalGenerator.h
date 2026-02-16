#pragma once

class SignalGeneratorConfiguration
{
public:
  float m_amplitude;
  float m_dc_shift;
  float m_frequency;
  float m_timesample;

public:
  SignalGeneratorConfiguration(float timesample,
                               float amplitude,
                               float frequency,
                               float dc_shift);
    
  ~SignalGeneratorConfiguration();  //Удаление конфигуратора после конфигурации

};//SignalGeneratorConfiguration


class SignalGenerator
{
protected:
  float m_output;
  float m_amplitude;
  float m_dc_shift;
  float m_frequency;
  float m_timesample;

public:
  SignalGenerator();

public:
  virtual void configure(SignalGeneratorConfiguration& configurator);
  virtual void execute();

public:
  float get_output();
};//SignalGenerator



