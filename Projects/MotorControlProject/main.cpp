//Библиотеки
#include "stm32f4xx.h"
#include <stdint.h>

//Файлы
#include "SystemInitialization.h"
#include "InterruptHeandlers.h"
#include "Observer.h"
#include "ADCHandler.h"
#include "FaultHandler.h"

#include "General.h"
#include "ControlSystem.h"


//Макросы
#define LED_D2_ON  GPIOA -> BSRR |= GPIO_BSRR_BR6;
#define LED_D2_OFF GPIOA -> BSRR |= GPIO_BSRR_BS6;

#define FPWM     float(5000.0)
#define TPWM     float(1.0/FPWM)
#define TSAMPLE  TPWM

//Глобальные переменные
int32_t us_counter   = 0; //Счётчик микросекунд (TIM4_Handler)
int32_t ms_counter   = 0; //Счётчик милисекунд
int32_t led_counter  = 0; //Счётчик секунд для светодиода (TIM2_Handler)

//Объекты-генераторы сигналов
SineWave        one_phase_sin_generator;
ThreeSineWave   three_phase_sin_generator;


//Наблюдатель
SynchroMotorObserver observer;

//Обработчик АЦП
ADCHandler adc_handler;

//Обработчик ошибок
FaultHandler fault_handler;

//Объект системы управления
ControlSystem motor_control_system;

//Конфигуратор системы управления
//Здесь произойдёт запись параметров частей САУ в их объекты-конфигураторы
ControlSystemConfiguration motor_control_system_configurator;

//Указатель на стратегию
ControlStrategy* currentStrategy = nullptr;

//Обработчик выбора текущего режима работы
StrategyHandler strategy_handler(&motor_control_system);


bool enable_work = false;
bool fault       = false;


int main()
{
  SystemInit();

  __disable_irq();//Запрет на прерывания

  RCC_Init();
  GPIO_Init();
  TIM1_Init();
  TIM2_Init();
  TIM4_Init();
  EXTI_Init();
  ADC1_DMA1_Init(); adc_handler.preparing_DMA();//Инициализация АЦП и памяти для DMA
  

  {//one-phase generator configuration
  SineWaveConfiguration one_phase_sin_generator_config(0.001, 5.0, 50.0, 5.0, 0.0);

  one_phase_sin_generator.configure(one_phase_sin_generator_config);
  }

  {//three-phase generator configuration
  ThreeSineWaveConfiguration three_phase_sin_generator_config(0.001, 5.0, 50.0, 5.0, 0.0);

  three_phase_sin_generator.configure(three_phase_sin_generator_config);
  }

  
  //Конфигурация наблюдателя
  SynchroMotorObserverConfigurator observer_configurator(TSAMPLE);

  /* Конфигурация САУ 
     Здесь произойдёт конфигурация всех составных частей САУ*/
  motor_control_system.configure(motor_control_system_configurator);

  //Разрешение прерываний
  __enable_irq();

  //Запуск счётчиков - начало работы программы, расчёта системы управления
  TIM_Start();

  
  while(1)
  {    
    /*Тут set(), reset() триггеров в fault_handler*/

    fault_handler.execute();

    /*Здесь в зависимости от внешних сигналов fault, enable_work (можно добавить ещё условия)
    происходит определение текущей стратегии-режима работы всего устройства в целом.
      В прерывании TIM2 по указателю currentStrategy выполняется 
    конкретная стратегия stop, work и иные. Можно добавлять другие режимы работы. В стратегиях можно 
    выполнять действия кроме рассчёта САУ (контакторы, обработчики ошибок и аварий и др.)
      Каждая стратегия имеет указатель на систему управления cs, по которому можно будет 
    вызывать различные режимы работы САУ, реализованные в соответствующих методах.
      Например, в режиме work просто вызывается метод motor_control_system.execute().
    */

    if(fault)            strategy_handler.set_stop();
    else if(enable_work) strategy_handler.set_work();
    else                 strategy_handler.set_stop();
    
    currentStrategy = strategy_handler.define_strategy();

  }//while(1)

  return 0;
}