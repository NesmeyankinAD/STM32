#include "stm32f4xx.h"
#include "ADCHandler.h"

void ADCHandler::preparing_DMA()
{
  DMA1_Stream0 -> M0AR = (uint32_t)&(ADC_data); //Указание адреса памяти, куда DMA будет складывать данные из АЦП

  DMA1_Stream0 -> CR |= DMA_SxCR_EN;            //Включение DMA
}

void ADCHandler::ADC_start()
{
  ADC1 -> CR2 |= ADC_CR2_SWSTART; //Запуск конвертаций АЦП
}

void ADCHandler::convert_data()
{
    //Пример от gpt
    ADC_data_converted[0] = (float)ADC_data[0] * (3.3f / 4095.0f);
    ADC_data_converted[1] = (float)ADC_data[1] * (3.3f / 4095.0f);
    ADC_data_converted[2] = (float)ADC_data[2] * (3.3f / 4095.0f);
}


//void ADC1_DMA1_Init()
//{
//  /*Abstract
//  Continuous measurment of phase currents by ADC1 and DMA1_Stream0. 
//  PC0 - IN10 
//  PC1 - IN11
//  PC2 - IN12
//  */
//
//  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;       //Тактирование GPIOC
//  RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;        //Тактирование ADC1
//  RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;        //Тактирование DMA1
//                                               
//                                               //Очистка регистра
//  GPIOC -> MODER &= ~(GPIO_MODER_MODER0_Msk |  
//                      GPIO_MODER_MODER1_Msk |  
//                      GPIO_MODER_MODER2_Msk);  
//                                               //PC0, PC1, PC2 - Analog mode
//  GPIOC -> MODER |=  (GPIO_MODER_MODER0 |                    
//                      GPIO_MODER_MODER1 |                    
//                      GPIO_MODER_MODER2);                    
//                                                             
//                                                             
//  DMA1_Stream0 -> CR = 0;                                    
//  DMA1_Stream0 -> PAR = (uint32_t)&(ADC1 -> DR);             //Периферийный адрес — регистр данных ADC
//  DMA1_Stream0 -> NDTR = 3;                                  //3 элемента (по 1 на канал)
//
//
//  DMA1_Stream0 -> CR |= DMA_SxCR_MINC;                       // инкремент addr при передаче
//  DMA1_Stream0 -> CR |= DMA_SxCR_PSIZE_0 | DMA_SxCR_MSIZE_0; // Размер данных — 16 бит
//  DMA1_Stream0 -> CR |= DMA_SxCR_CIRC;                       // Круговой режим
//  DMA1_Stream0 -> CR |= (0 << DMA_SxCR_CHSEL_Pos);           // Канал 0
//  DMA1_Stream0 -> CR |= DMA_SxCR_DIR_0;                      // Периферия -> Память
//
//  {//Вынести в отдельную функцию, инициализирующую буфер данных АЦП
//  //DMA1_Stream0 -> M0AR = (uint32_t)&adcBuffer;
//  //// Включение DMA
//  //DMA1_Stream0->CR |= DMA_SxCR_EN;
//  }
//
//  ADC1 -> CR1 = 0;              
//  ADC1 -> CR2 = 0;              
//                                
//  ADC1 -> CR1 |= ADC_CR1_SCAN;             //Включить сканирование
//                                           
//  ADC1 -> CR2 |= ADC_CR2_CONT;             //Непрерывный режим
//  ADC1 -> CR2 |= ADC_CR2_DMA;              //Включить DMA
//                                           
//  ADC1 -> SQR1 &= ~(ADC_SQR1_L);           
//  ADC1 -> SQR1 |= (2 << ADC_SQR1_L_Pos);   //Длина последовательности преобразований 3 канала - 0,1,2
//                                           
//  ADC1 -> SMPR1 &= ~(ADC_SMPR1_SMP10_Msk | 
//                     ADC_SMPR1_SMP11_Msk | 
//                     ADC_SMPR1_SMP12_Msk); //Очистка и установка sampling time = 3 samples
//                                           
//  ADC1 -> CR2 |= ADC_CR2_ADON;             //Включение АЦП, но не запуск конвертаций
//
//
//
//  {//Вынести в отдельную функцию, инициализирующую буфер данных АЦП
//
//  //DMA1_Stream0 -> M0AR = (uint32_t)&adcBuffer;//Указание адреса памяти, куда DMA будет складывать данные из АЦП
//
//  //DMA1_Stream0->CR |= DMA_SxCR_EN;            //Включение DMA
//
//  //ADC1->CR2 |= ADC_CR2_SWSTART;               //Запуск конвертаций АЦП
//  }
//} 
