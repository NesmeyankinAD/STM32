#include "stm32f4xx.h"
#include "DMA_Init.h"

uint16_t buffer[8] = {0,0,0,0,0,0,0,0};

void DMA2_Stream0_Init(void)
{
/*Abstract
  DMA2 используем для отправки оцифрованных значений ADC1 в регистр TIM1:CCR1. 
  Для этого использем Stream0 Channel0 DMA2.
*/

  RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;        //Включение тактирования DMA2

  DMA2_Stream0 -> PAR = (uint32_t)&(ADC1->DR); //Адрес периферии, откуда забираем
  
  DMA2_Stream0 -> M0AR = (uint32_t)buffer;     //Адрес памяти, куда кладём
  
  DMA2_Stream0 -> NDTR = 8;                    //Количество передаваемых данных
  
  DMA2_Stream0 -> FCR &= ~(DMA_SxFCR_DMDIS);   //Прямой режим, без FIFO

  DMA2_Stream0 -> CR  &= ~(DMA_SxCR_CHSEL);    //Channel 0
  DMA2_Stream0 -> CR  &= ~(DMA_SxCR_MBURST);   //Одиночная передача
  DMA2_Stream0 -> CR  &= ~(DMA_SxCR_PBURST);   //Одиночная передача
  DMA2_Stream0 -> CR  &= ~(DMA_SxCR_DBM);      //Режим двойного буфера выключен
  DMA2_Stream0 -> CR  |= DMA_SxCR_PL;          //11 - Наивысший приоритет
  DMA2_Stream0 -> CR  |= DMA_SxCR_MSIZE_0;     //01 - Размер данных памяти 16-бит
  DMA2_Stream0 -> CR  |= DMA_SxCR_PSIZE_0;     //01 - Размер данных периферии 16-бит
  DMA2_Stream0 -> CR  |= DMA_SxCR_MINC;        //Включение инкрементирования адреса памяти
  DMA2_Stream0 -> CR  &= ~(DMA_SxCR_PINC);     //Выключение инкрементирования адреса периферии
  DMA2_Stream0 -> CR  |= DMA_SxCR_CIRC;        //Циклический режим работы
  DMA2_Stream0 -> CR  &= ~(DMA_SxCR_DIR);      //Направление передачи из периферии в память
  DMA2_Stream0 -> CR  |= DMA_SxCR_TCIE;        //Включение прерывания по окончанию передачи

  NVIC_EnableIRQ(DMA2_Stream0_IRQn);           //Разрешение прерываний DMA2 в NVIC

  DMA2_Stream0 -> CR  |= DMA_SxCR_EN;          //Запуск DMA2 Stream0
}
