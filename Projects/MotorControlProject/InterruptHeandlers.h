#include "stm32f4xx.h"
#include "SineWave.h"
#include "ThreeSineWave.h"
#include "Observer.h"

#include "General.h"

#define LED_D2_ON  GPIOA -> BSRR |= GPIO_BSRR_BR6;
#define LED_D2_OFF GPIOA -> BSRR |= GPIO_BSRR_BS6;

extern int32_t   us_counter;
extern int32_t   ms_counter;
extern int32_t   led_counter;

extern SynchroMotorObserver observer;

extern ControlStrategy* currentStrategy;

extern SineWave        one_phase_sin_generator;
extern ThreeSineWave   three_phase_sin_generator;

extern "C" void SysTick_Handler();

extern "C" void TIM2_IRQHandler();

extern "C" void TIM4_IRQHandler();

extern "C" void ADC_IRQHandler();

extern "C" void EXTI1_IRQHandler();
extern "C" void EXTI2_IRQHandler();
extern "C" void EXTI3_IRQHandler();