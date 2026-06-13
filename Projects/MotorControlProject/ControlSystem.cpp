#include "ControlSystem.h"

ControlSystem::ControlSystem(ADCHandler* adc_external, SynchroMotorObserver* observer_external)
{
  adc      = adc_external;
  observer = observer_external;
}

void ControlSystem::configure(ControlSystemConfiguration& configurator)
{
  /*Debug
  PI_current_loop.configure(configurator.PI_current_loop_configurator);
  PI_Udc_loop.configure(configurator.PI_Udc_loop_configurator);
  one_phase_sin_generator.configure(configurator.one_phase_sin_generator_config);
  */
  three_phase_generator.configure(configurator.three_phase_generator_config);

  PI_I_d.configure(configurator.PI_I_d_configurator);
  PI_I_q.configure(configurator.PI_I_q_configurator);
  PI_w.configure(configurator.PI_w_configurator);
  PI_angle.configure(configurator.PI_angle_configurator);
  angle_calc.configure(configurator.angle_calc_configurator);
}

void ControlSystem::speed_calc()
{
  // Константы
  #define STABLE_TIME_TICKS 500     //500 - 0.1с 
                                    //Пороговое кол-во тактов, за которые w в observer не изменилась
  #define SPEED_EPSILON     0.001f  //Порог стабильности

  // Состояние
  static float    last_observed_speed = 0.0f;
  static uint32_t stable_counter = 0;
  static bool     speed_is_zeroed = false;

  float current_speed = observer->get_rotation_frequency();

  if (speed_is_zeroed) 
  {
      // Если скорость была обнулена, проверяем, появилось ли движение
      if (fabs(current_speed - last_observed_speed) > SPEED_EPSILON) 
      {
          // Скорость изменилась - выходим из режима обнуления
          speed_is_zeroed = false;
          w_motor = current_speed;
          last_observed_speed = current_speed;
          stable_counter = 0;
      } else 
      {
          // Скорость остается стабильной - держим обнуленной
          w_motor = 0.0f;
      }
  } 
  else 
  {
      // Нормальный режим - отслеживаем стабильность
      if (fabs(current_speed - last_observed_speed) <= SPEED_EPSILON) 
      {
          // Скорость стабильна
          stable_counter++;
        
          if (stable_counter >= STABLE_TIME_TICKS) 
          {
              // Скорость стабильна 1 секунду - обнуляем
              w_motor = 0.0f;
              speed_is_zeroed = true;
              // last_observed_speed сохраняем, чтобы отслеживать изменения
              stable_counter = 0;
          } 
          else 
          {
              // Еще не набрали нужное время - используем текущую скорость
              w_motor = current_speed;
          }
      } 
      else 
      {
          // Скорость изменилась - сбрасываем счетчик
          w_motor = current_speed;
          last_observed_speed = current_speed;
          stable_counter = 0;
      }
  }
}

float ControlSystem::continuous_angle(float w)
{
  static float last_observer_angle = 0.0;
  static float temp_angle = 0.0;

  if(last_observer_angle == observer -> get_angle())
  {
    //Если угол в observer не изменился, то вычисляем угол интегрирированием
    angle_calc.execute(w);
    temp_angle = angle_calc.get_output();
  }
  else
  {
    //Угол в observer изменился, тогда:
    //1. возвращаем актуальный угол из observer
    //2. в выход интегратора присваиваем актуальный угол

    last_observer_angle = observer -> get_angle();
    temp_angle = last_observer_angle;

    angle_calc.reset();
    angle_calc.set_initial_output(last_observer_angle);
  }

  return temp_angle;
}

void ControlSystem::starter()
{
  angle_motor = continuous_angle(180.0);
  angle_field = 10.0 * angle_motor;
  
  I_q_motor_ref = 1.5;
}


void ControlSystem::execute()
{
  TIM1_Start();

  GPIOC -> BSRR &= ~(1 << 20); //PWM Enable - 0
  
  /*//Udc control
  //PI_Udc_loop.execute(Udc_ref, adc -> ADC_data_converted[3]);
  //
  //PI_current_loop.execute(PI_Udc_loop.get_output(), adc -> ADC_data_converted[0]);
  //
  //TIM1 -> CCR1 = (uint32_t)((PI_current_loop.get_output()/100.0) * 16800.0);
  //*/
  /*//AC voltage generation
  one_phase_sin_generator.execute();

  float cmp_direct  = (0.5 * (one_phase_sin_generator.get_output()/20.0 + 1) * 16800.0);

  float cmp_inverse = (16800.0 - (0.5 * (one_phase_sin_generator.get_output()/20.0 + 1) * 16800.0));

  TIM1 -> CCR1 = (uint32_t)cmp_direct;
  TIM1 -> CCR2 = (uint32_t)cmp_inverse;

  //*/


  //---Контур скорости---//
  
  speed_calc();
  
  
  PI_w.execute(w_ref, w_motor);

  if(w_motor == 0.0 || abs(observer->get_angle()) < 18.0)
  {
    //Двигатель стоит
    if(abs(w_ref) != 0) starter();
  }
  else
  {
    angle_motor = continuous_angle(w_motor);
    angle_field = 10.0 * angle_motor;
    
    I_q_motor_ref = PI_w.get_output();
  }
  

  /*
  angle_calc.execute(90.0);
  angle_motor = angle_calc.get_output();
  angle_field = 10.0 * angle_motor;
  I_q_motor_ref = 3.0;
  */


  //---Контур скорости---//

  //---Контуры тока---//
  coordinate_transform.ABC_dq_transform((angle_field) * M_PI/180,
                                        adc -> ADC_data_converted[0],
                                        adc -> ADC_data_converted[1],
                                        adc -> ADC_data_converted[2], 
                                        I_d_motor,
                                        I_q_motor);
  PI_I_d.execute(I_d_motor_ref, I_d_motor);
  PI_I_q.execute(I_q_motor_ref, I_q_motor);
  //---Контуры тока---//
  
  
  coordinate_transform.dq_ABC_transform((angle_field) * M_PI/180,
                                        PI_I_d.get_output(),
                                        PI_I_q.get_output(),
                                        U_phases[0],
                                        U_phases[1],
                                        U_phases[2]);
  
  modulator.execute(U_phases, U_maxmin);
  
  
  cmp[0] = (0.5f * (U_phases[0] / U_DC + 1.0f) * 16800.0f);
  cmp[1] = (0.5f * (U_phases[1] / U_DC + 1.0f) * 16800.0f);
  cmp[2] = (0.5f * (U_phases[2] / U_DC + 1.0f) * 16800.0f);
  
  TIM1 -> CCR1 = (uint32_t)cmp[0];
  TIM1 -> CCR2 = (uint32_t)cmp[1];
  TIM1 -> CCR3 = (uint32_t)cmp[2];
  

  /*
  three_phase_generator.execute();
  cmp[0] = (0.5f * (three_phase_generator.get_output_A() / U_DC + 1.0f) * 16800.0f);
  cmp[1] = (0.5f * (three_phase_generator.get_output_B() / U_DC + 1.0f) * 16800.0f);
  cmp[2] = (0.5f * (three_phase_generator.get_output_C() / U_DC + 1.0f) * 16800.0f);
  
  TIM1 -> CCR1 = (uint32_t)cmp[0];
  TIM1 -> CCR2 = (uint32_t)cmp[1];
  TIM1 -> CCR3 = (uint32_t)cmp[2];
  */

  DAC -> DHR12R1 = (0.5f * (U_phases[0] / U_DC + 1.0f) * 3500);
  
}

void ControlSystem::stop()
{
  TIM1 -> CCR1 = 0;
  TIM1 -> CCR2 = 0;
  TIM1 -> CCR3 = 0;
  TIM1_Stop();

  GPIOE -> BSRR &= ~(0x3F << 24); //~(0011 1111 0000 0000 0000 0000 0000 0000)
                                  //Ручное обнуление ШИМ-пинов PE8-PE13
}