#pragma once
#include "stm32f4xx.h"

extern uint16_t buffer[8] __attribute__((section(".fast")));

void DMA2_Stream0_Init(void);