#include "Observer.h"

SynchroMotorObserverConfigurator::
SynchroMotorObserverConfigurator(float timesample)
{
  m_timesample = timesample;
}

SynchroMotorObserver::SynchroMotorObserver():
  m_rotation_direction  (0),
  m_angle_counter       (0),
  m_turns_counter       (0),
  last_position         (0),
  current_position      (0),
  m_previous_time       (0.0),
  m_present_time        (0.0),
  m_angle               (0.0),
  m_rotation_frequency  (0.0)
{}

void SynchroMotorObserver::configure(SynchroMotorObserverConfigurator& configurator)
{
  m_timesample = configurator.m_timesample;
}

float SynchroMotorObserver::get_angle()
{
  return m_angle;
}

int32_t SynchroMotorObserver::get_turns_number()
{
  return m_turns_counter;
}

float SynchroMotorObserver::get_rotation_frequency()
{
  return m_rotation_frequency;
}

int8_t SynchroMotorObserver::get_rotation_direction()
{
  return m_rotation_direction;
}

void SynchroMotorObserver::set_previous_time(int32_t& time)
{
  m_previous_time = time;
}

void SynchroMotorObserver::set_present_time (int32_t& time)
{
  m_present_time = time;
}

void SynchroMotorObserver::compute_rotation_frequency()
{
  m_rotation_frequency = (m_rotation_direction * 6.0 / ((m_present_time - m_previous_time) / 1000000.0) / 1); 
}

void SynchroMotorObserver::compute_angle()
{
  if(m_rotation_direction == 1)
  {
    m_angle_counter++;

    if(m_angle_counter % 60 == 0) {m_turns_counter++;}
  }
  else
  if(m_rotation_direction == -1)
  {
    m_angle_counter--;

    if(m_angle_counter % 60 == 0) {m_turns_counter--;}
  }
  else
  {}

  if(m_angle_counter == 60 || m_angle_counter == -60) m_angle_counter = 0;

  m_angle = m_turns_counter * 360 + m_angle_counter * 6;
}

void SynchroMotorObserver::compute_rotation_direction()
{
  /*Abstract
PA1 - yellow, 
PA2 - green, 
PA3 - blue.
GPIOA -> IDR = 0b1110 - PA1 = PA2 = PA3 = 1, PA0 = 0.
*/

  current_position = (int8_t(GPIOA -> IDR)) & 0b00001110;

  if(last_position)
  {
    if(
       ((last_position == 0b1000) && (current_position == 0b1010)) ||
       ((last_position == 0b1010) && (current_position == 0b0010)) ||
       ((last_position == 0b0010) && (current_position == 0b0110)) ||
       ((last_position == 0b0110) && (current_position == 0b0100)) ||
       ((last_position == 0b0100) && (current_position == 0b1100)) ||
       ((last_position == 0b1100) && (current_position == 0b1000))
      )
      {m_rotation_direction = 1; /*direct rotation*/}
    else 
    if(
       ((last_position == 0b0010) && (current_position == 0b1010)) ||
       ((last_position == 0b1010) && (current_position == 0b1000)) ||
       ((last_position == 0b1000) && (current_position == 0b1100)) ||
       ((last_position == 0b1100) && (current_position == 0b0100)) ||
       ((last_position == 0b0100) && (current_position == 0b0110)) ||
       ((last_position == 0b0110) && (current_position == 0b0010)) 
       )
       {m_rotation_direction = -1; /*inverse rotation*/}

    last_position = current_position;
  }
  else
  {
    last_position = current_position;
    m_rotation_direction = 0;
  }
}
