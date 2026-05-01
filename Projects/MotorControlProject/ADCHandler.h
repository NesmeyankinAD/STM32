#pragma once

#include <stdint.h>
#include "Delay.cpp"


class ADCHandler
{
public:
  uint16_t* ADC_data; //Буфер для DMA

  //За период ШИМ храним 4 измерения
  uint16_t ADC_data_A[4]{0};            //Буфер для тока фазы А
  uint16_t ADC_data_B[4]{0};            //Буфер для тока фазы B
  uint16_t ADC_data_C[4]{0};            //Буфер для тока фазы C

  uint16_t ADC_data_Udc[4]{0};          //Буфер для Udc

  float    ADC_data_converted[4]{0.0};  //Обработанные данные
  
public:
  ADCHandler(uint16_t* buffer) : ADC_data(buffer){};

public:
  void preparing_DMA();
  void ADC_start();
  void ADC_stop();

public:
  void copy_data();
  void convert_data();
        
};//ADCHandler