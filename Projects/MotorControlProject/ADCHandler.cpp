#include "stm32f4xx.h"
#include "ADCHandler.h"


void ADCHandler::preparing_DMA()
{
  DMA2_Stream0 -> M0AR = (uint32_t)ADC_data; //Указание адреса памяти, куда DMA будет складывать данные из АЦП

  //DMA2_Stream0 -> CR |= DMA_SxCR_EN;            //Включение DMA
}

void ADCHandler::ADC_start()
{
// ✅ 1. Остановить DMA если он был включен
  DMA2_Stream0 -> CR &= ~DMA_SxCR_EN;
  
  // Ждать остановки DMA (с таймаутом для безопасности)
  volatile int timeout = 10000;
  while((DMA2_Stream0 -> CR & DMA_SxCR_EN) && (--timeout > 0)) 
  { 
    __NOP(); 
  }
  
  // ✅ 2. Сбросить ВСЕ флаги DMA Stream 0
  DMA2 -> LIFCR = DMA_LIFCR_CTCIF0   |  // Transfer Complete
                  DMA_LIFCR_CHTIF0   |  // Half Transfer
                  DMA_LIFCR_CTEIF0   |  // Transfer Error
                  DMA_LIFCR_CFEIF0;     // FIFO Error
  
  // ✅ 3. Перезагрузить количество данных
  DMA2_Stream0 -> NDTR = 3;
  
  // ✅ 4. Включить DMA
  DMA2_Stream0 -> CR |= DMA_SxCR_EN;
  
  // ✅ 5. Убедиться, что ADC включен
  if((ADC1 -> CR2 & ADC_CR2_ADON) == 0)
  {
    ADC1 -> CR2 |= ADC_CR2_ADON;
    for(volatile int i = 0; i < 100; i++) { __NOP(); }
  }
  
  // ✅ 6. Запустить ADC конверсию
  ADC1 -> CR2 |= ADC_CR2_SWSTART;

  //ADC1 -> CR2 |= ADC_CR2_ADON;
  //
  //int i = 100;
  //while(i--) {__NOP();}
  //
  //ADC1 -> CR2 |= ADC_CR2_SWSTART; //Запуск конвертаций АЦП
}

void ADCHandler::ADC_stop()
{
  ADC1 -> CR2 &= ~ADC_CR2_ADON;
}

void ADCHandler::copy_data()
{
  static int iterator = 0;

  if(iterator == 4) iterator = 0;

  ADC_data_A[iterator] = ADC_data[0];
  ADC_data_B[iterator] = ADC_data[1];
  ADC_data_C[iterator] = ADC_data[2];

  iterator++; 
}

void ADCHandler::convert_data()
{
    //Фазные токи, усреднение и вычисление
    //В схеме измерения есть смещение на 3.3/2 = 1.65 В (2048 уровней)
    int avg_A = (ADC_data_A[0] + ADC_data_A[1] + ADC_data_A[2] + ADC_data_A[3] - 2048 * 4) / 4;
    int avg_B = (ADC_data_B[0] + ADC_data_B[1] + ADC_data_B[2] + ADC_data_B[3] - 2048 * 4) / 4;
    int avg_C = (ADC_data_C[0] + ADC_data_C[1] + ADC_data_C[2] + ADC_data_C[3] - 2048 * 4) / 4;
    
    /*
                 (avg/2048) * 1.65
    i_phase = -----------------------; Ku_measurment = 20; R_shunt = 0.005 Ohm.
              R_shunt * Ku_measurment
    */
    
    ADC_data_converted[0] = (((float)avg_A / 2048.0) * 1.65) / (0.005 * 20.0);
    ADC_data_converted[1] = (((float)avg_B / 2048.0) * 1.65) / (0.005 * 20.0);
    ADC_data_converted[2] = (((float)avg_C / 2048.0) * 1.65) / (0.005 * 20.0);
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
