#include "stm32f4xx.h"

static inline void delay_us_simple(uint32_t us)
{
  volatile uint32_t iterations = (SystemCoreClock / 1000000) * us / 5;
  while (iterations--) { __NOP(); }
}