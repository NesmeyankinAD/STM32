#include "stm32f4xx.h"
#include "Integrator.h"
#include "Meander.h"
#include "RisingSaw.h"
#include "FallingSaw.h"
#include "DoublePitchSaw.h"
#include "SineWave.h"
#include "ThreeSineWave.h"

#define LED_D2_ON  GPIOA -> BSRR |= GPIO_BSRR_BR6;
#define LED_D2_OFF GPIOA -> BSRR |= GPIO_BSRR_BS6;

extern uint16_t   ms_counter;
extern uint16_t   led_counter;
extern uint16_t   duty;

extern Integrator integrator_1;
extern float      integratorScope;

extern Meander         meander_generator;
extern RisingSaw       rising_saw_generator;
extern FallingSaw      falling_saw_generator;
extern DoublePitchSaw  double_pitch_saw_generator;
extern SineWave        one_phase_sin_generator;
extern ThreeSineWave   three_phase_sin_generator;

extern "C" void SysTick_Handler();

extern "C" void TIM2_IRQHandler();

extern "C" void ADC_IRQHandler();