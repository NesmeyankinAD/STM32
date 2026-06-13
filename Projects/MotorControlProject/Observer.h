#pragma once

#include <stdint.h>
#include "stm32f4xx.h"

class SynchroMotorObserverConfigurator
{
public:
  float m_timesample;

public:
  SynchroMotorObserverConfigurator(float timesample);

};//SynchroMotorObserverConfigurator


class SynchroMotorObserver
{
private:
  float m_timesample;
  
  volatile int8_t  m_rotation_direction; //1 - direct, -1 - inverse, 0 - stop
  volatile int32_t m_angle_counter;
  volatile int32_t m_turns_counter;

  volatile int8_t last_position;
  volatile int8_t current_position;

  float m_previous_time;
  float m_present_time;

  float m_previous_angle;

  float m_angle;
  float m_rotation_frequency;

public:
  SynchroMotorObserver();

public:
  void configure(SynchroMotorObserverConfigurator& configurator);

public:
  float   get_angle();
  float   get_previous_angle();
  int32_t get_turns_number();
  float   get_rotation_frequency();
  int8_t  get_rotation_direction();
  float   get_previous_time();
  
public:
  void set_previous_time(int32_t& time);
  void set_present_time (int32_t& time);

  void set_previous_angle(float angle);

  void compute_rotation_frequency();
  void compute_angle_6();
  void compute_angle_12();
  void compute_rotation_direction_6();
  void compute_rotation_direction_12();
  
};//SynchroMotorObserver