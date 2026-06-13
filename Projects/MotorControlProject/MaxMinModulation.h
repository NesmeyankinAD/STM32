#pragma once

#include <algorithm>
#include <vector>

#define coef 1.1547005383792

class MaxMinModulation
{
public:
  void execute(std::vector<float> phases, float& A, float&B, float&C);
  
  void execute(std::vector<float> phases, std::vector<float> max_min);

  void execute(float* phases, float* max_min);

};//MaxMinModulation