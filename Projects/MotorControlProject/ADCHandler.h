#pragma once

#include <stdint.h>

class ADCHandler
{
public:
  uint16_t ADC_data[3]{0};
  float    ADC_data_converted[3]{0.0};
  
public:
  void preparing_DMA();
  void ADC_start();

public:
  void convert_data();
        
};//ADCHandler