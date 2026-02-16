#include "stm32f4xx.h"

void ADC1_Init(void)
{
  /* ADC1 IN2 - GPIO A2 */

  /*---Тактирование и порты АЦП---*/
  {
    RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;    //Тактирование АЦП1
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   //Тактирование GPIOA АЦП1
    
    GPIOA -> MODER |= GPIO_MODER_MODE2;      //GPIO_A2 - аналоговый режим
  }
  
  /*---Настройка АЦП---*/
  {
    ADC1 -> SMPR2 |= ADC_SMPR2_SMP2_0;       //001 - 15 циклов тактирования АЦП (sample time)
                                             //Длительность одной конвертации 12 + sample time = 27 циклов тактирования АЦП

    ADC1 -> SQR1  &= ~(ADC_SQR1_L);          //Длина последовательности конвертаций = 1
    
    ADC1 -> SQR3  |= (2 << ADC_SQR3_SQ1_Pos);//Номер канала АЦП в последовательности конвертаций SQR
                                             //Канал №2 АЦП1 первый в последовательности конвертаций SQR
    
    //ADC1 -> CR1   |= ADC_CR1_EOCIE;          //Разрешение прерывания по окончанию преобразования
    //NVIC_EnableIRQ(ADC_IRQn);                //Разрешение прерываний в контроллере NVIC

    ADC1 -> CR2   |= ADC_CR2_CONT;           //Непрерывный режим работы АЦП1
    ADC1 -> CR2   |= ADC_CR2_EOCS;           //Прерывание EOCI вызывается после каждой конвертации в последовательности
    
    ADC1 -> CR2   |= ADC_CR2_DMA;            //Включение DMA для ADC1
    ADC1 -> CR2   |= ADC_CR2_DDS;            //Запрос на DMA после каждой конвертации

    ADC1 -> CR2   |= ADC_CR2_ADON;           //Включение АЦП
    ADC1 -> CR2   |= ADC_CR2_SWSTART;        //Включение конвертаций АЦП
  }



}