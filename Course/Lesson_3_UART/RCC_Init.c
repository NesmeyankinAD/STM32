#include "stm32f4xx.h"

void RCC_Init(void)
{
  RCC -> CR |= RCC_CR_HSEON;                    // Включение внешнего источника тактового сигнала (кварц 8 МГц)
  while ( (RCC -> CR & RCC_CR_HSERDY) == 0 ) {} // Ожидание стабилизации тактового сигнала
                                                // (ожидание установки бита HSERDY в регистре CR)

  RCC -> CR &= ~(RCC_CR_PLLON);                 // Отключение PLL перед настройкой тактирования (явная установка 0)
  RCC -> PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;     // Выбор HSE в качестве источника тактирования

/* Пояснения к настройке тактирования 

F_HSE_CLK         = 8 MHz;  (кварцевый резонатор)
F_PLL_CLK_OUT_REF = 100 MHz;(требуемая выходная частота PLL)

F_VCO_CLK     = F_HSE_CLK * (PLLN / PLLM);
F_PLL_CLK_OUT = F_VCO_CLK / PLLP;

                                PLLN
F_PLL_CLK_OUT = F_HSE_CLK * -----------;
                            PLLP * PLLM

Ограничения:
F_PLL_CLK_OUT <= 168 MHz;
F_VCO_CLK     <= 432 MHz;

64 <= PLLN <= 432;
2  <= PLLM <= 63;
------
                    100
100 MHz = 8 MHz * -------;
                   2 * 4

  PLLP = 2;   (00)
  PLLN = 100; (0110 0100)
  PLLM = 4;   (00 0100)

SYSCLK = 100 MHz;
-----

HCLK = SYSCLK / AHB_PRESC;

APB1_PRPH_CLK  =  HCLK / APB1_PRESC;
APB1_PRPH_CLK  <= 42 MHz;
APB1_TIMER_CLK = APB1_PRPH_CLK * 2;

APB2_PRPH_CLK  =  HCLK / APB2_PRESC;
APB2_PRPH_CLK  <= 84 MHz;
APB2_TIMER_CLK = APB2_PRPH_CLK * 2;
-----

SYSCLK = 100 MHz;

AHB_PRESC = 1;

HCLK = 100 MHz / 1 = 100 MHz;

APB1_PRESC     = 4;
APB1_PRPH_CLK  = 100 MHz / 4 = 25 MHz; 
APB1_TIMER_CLK = 25 MHz * 2 = 50 MHz;

APB2_PRESC     = 2;
APB2_PRPH_CLK  = 100 MHz / 2 = 50 MHz;
APB2_TIMER_CLK = 50 MHz * 2  = 100 MHz;
----

Требуемые настройки тактирования:

RCC -> PLLCFG:
1. PLLP = 2;   (00)
2. PLLN = 100; (0110 0100)
3. PLLM = 4;   (00 0100)

RCC -> CFGR:
4. AHB_PRESC  = 1;
5. APB1_PRESC = 4;
6. APB2_PRESC = 2;

*/
 
/*------------------------------------------------------------------------------------------- */
    // PLLM = 000100;
    RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLM); // Очистка регистра в области PLLM
    RCC -> PLLCFGR |= RCC_PLLCFGR_PLLM_2;  // Запись 1 побитово для каждого разряда
    
    // PLLN = 0110 0100; 
    RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
    RCC -> PLLCFGR |= RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_6;
    
    // PLLP = 00;
    RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
/*------------------------------------------------------------------------------------------- */
    // AHB Prescaler = 1;
    RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;
    
    // APB1 Prescaler = 4;
    RCC -> CFGR |= RCC_CFGR_PPRE1_DIV4;
    
    // APB2 Prescaler = 2;
    RCC -> CFGR |= RCC_CFGR_PPRE2_DIV2;
/*------------------------------------------------------------------------------------------- */

 RCC -> CR |= RCC_CR_PLLON; // Включение PLL

 while ( (RCC->CR & RCC_CR_PLLRDY) == 0 ) {} // Ожидание стабилизации частоты PLL

/* Настройка регистров FLASH-памяти.
Эта операция обязательна и выполняется только 1 раз при старте программы.*/
/*------------------------------------------------------------------------------------------- */
 FLASH -> ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS | FLASH_ACR_PRFTEN;
/*------------------------------------------------------------------------------------------- */
 
 RCC -> CFGR &= ~RCC_CFGR_SW;
 RCC -> CFGR |= RCC_CFGR_SW_PLL;  // Подключение PLL к SYSCLK.
                                  // Выбор PLL в качестве основного источника тактирования

 while ( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL ) {} // Ожидание, пока PLL подключится к SYSCLK

 SystemCoreClockUpdate();
}