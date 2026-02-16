#include "ThreeSineWave.h"

ThreeSineWaveConfiguration::
ThreeSineWaveConfiguration(float timesample,
                           float amplitude,
                           float frequency,
                           float dc_shift,
                           float phase_shift_grad):
SignalGeneratorConfiguration::
SignalGeneratorConfiguration(timesample, amplitude, frequency, dc_shift)
{
  m_phase_shift_grad = phase_shift_grad;
}

ThreeSineWave::ThreeSineWave() : SignalGenerator()
{
  m_output_A = 0.0;
  m_output_B = 0.0;
  m_output_C = 0.0;
}

void ThreeSineWave::configure(ThreeSineWaveConfiguration& configurator)
{
  m_timesample = configurator.m_timesample;
  m_amplitude  = configurator.m_amplitude;
  m_frequency  = configurator.m_frequency;
  m_dc_shift   = configurator.m_dc_shift;
  m_phase_shift_grad = configurator.m_phase_shift_grad;

  SineWaveConfiguration phase_A_config(m_timesample,
                                       m_amplitude,
                                       m_frequency,
                                       0,
                                       m_phase_shift_grad);

  SineWaveConfiguration phase_B_config(m_timesample,
                                       m_amplitude,
                                       m_frequency,
                                       0,
                                       m_phase_shift_grad - (2.0/3.0)*M_PI);

  SineWaveConfiguration phase_C_config(m_timesample,
                                       m_amplitude,
                                       m_frequency,
                                       0,
                                       m_phase_shift_grad - (4.0/3.0)*M_PI);
  phase_A.configure(phase_A_config);
  phase_B.configure(phase_B_config);
  phase_C.configure(phase_C_config);
}

void ThreeSineWave::execute()
{
  static float exec_result_A = 0.0;
  static float exec_result_B = 0.0;
  static float exec_result_C = 0.0;

  phase_A.execute();
  phase_B.execute();
  phase_C.execute();

  exec_result_A = phase_A.get_output();
  exec_result_B = phase_B.get_output();
  exec_result_C = phase_C.get_output();

  m_output_A = exec_result_A + m_dc_shift;
  m_output_B = exec_result_B + m_dc_shift;
  m_output_C = exec_result_C + m_dc_shift;
}

void ThreeSineWave::reset()
{
  m_output_A = 0.0;
  m_output_B = 0.0;
  m_output_C = 0.0;
}

float ThreeSineWave::get_output_A()
{
  return m_output_A;
}

float ThreeSineWave::get_output_B()
{
  return m_output_B;
}

float ThreeSineWave::get_output_C()
{
  return m_output_C;
}