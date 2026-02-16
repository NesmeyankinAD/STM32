#include "stm32f4xx.h"
#include "Integrator.h"

#define LED_D2_ON  GPIOA -> BSRR |= GPIO_BSRR_BR6;
#define LED_D2_OFF GPIOA -> BSRR |= GPIO_BSRR_BS6;

extern uint16_t   ms_counter;
extern uint16_t   led_counter;
extern Integrator integrator_1;
extern float      integratorScope;
extern uint16_t   duty;

void SysTick_Handler();

void TIM2_IRQHandler();

void ADC_IRQHandler();