//Библиотеки
#include "stm32f4xx.h"

//Файлы
#include "SystemInitialization.h"
#include "InterruptHeandlers.h"


//Макросы
#define LED_D2_ON  GPIOA -> BSRR |= GPIO_BSRR_BR6;
#define LED_D2_OFF GPIOA -> BSRR |= GPIO_BSRR_BS6;

//Глобальные переменные
uint16_t ms_counter = 0; //Счётчик милисекунд
uint16_t led_counter = 0;
uint16_t duty = 0;

  //Объекты-генераторы сигналов
Meander         meander_generator;
RisingSaw       rising_saw_generator;
FallingSaw      falling_saw_generator;
DoublePitchSaw  double_pitch_saw_generator;
SineWave        one_phase_sin_generator;
ThreeSineWave   three_phase_sin_generator;


int main()
{
  SystemInit();

  RCC_Init();
  GPIO_Init();
  TIM1_Init();
  TIM2_Init();
  DAC_Init();
  
  //SysTick_Config(168000);//Прерывание по SysTick каждые 1мс.

  {//integrator configuration
  //IntegratorConfiguration integratorConfigurator(1, 1.0/1000.0, 5.0, -10000.0);

  //integrator_1.configure(integratorConfigurator);
  }

  {//meander configuration
    MeanderConfiguration meander_generator_config(0.001,
                                                  5.0,
                                                  50,
                                                  5.0,
                                                  50.0);

    meander_generator.configure(meander_generator_config);
  }

  {//rising saw configuration
  //RisingSawConfiguration  rising_saw_generator_config(0.001, 5.0, 50.0, 1.0);
  //
  //rising_saw_generator.configure(rising_saw_generator_config);
  }

  {//falling saw configuration
  FallingSawConfiguration falling_saw_generator_config(0.001, 5.0, 50.0, 0.0);

  falling_saw_generator.configure(falling_saw_generator_config);
  }

  {//double-pitch saw configuration
  DoublePitchSawConfiguration double_pitch_saw_generator_config(0.001, 5.0, 0.5, 0.0);
  
  double_pitch_saw_generator.configure(double_pitch_saw_generator_config);
  }

  {//one-phase generator configuration
  SineWaveConfiguration one_phase_sin_generator_config(0.001, 5.0, 50.0, 5.0, 0.0);

  one_phase_sin_generator.configure(one_phase_sin_generator_config);
  }

  {//three-phase generator configuration
  ThreeSineWaveConfiguration three_phase_sin_generator_config(0.001, 5.0, 50.0, 5.0, 0.0);

  three_phase_sin_generator.configure(three_phase_sin_generator_config);
  }


  TIM_Start();//Запуск счётчиков - начало работы программы

  //Бесконечный цикл
  while(1)
  {
    
  }

  return 0;
}