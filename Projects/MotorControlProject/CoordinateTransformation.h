#pragma once

#include <math.h>
#include <stdint.h>

#define SQRT_3 1.7320508075688

class CoordinateTrasformation
{
public:
  void ABC_ab_transform(float A, float B, float C, float& a, float& b);
  void ab_ABC_transform(float a, float b, float& A, float& B, float& C);

  void ab_dq_transform(float a, float b, float& d, float& q);
  void dq_ab_transform(float d, float q, float& a, float& b);
  
  void ABC_dq_transform(float A, float B, float C, float& d, float& q);
  void dq_ABC_transform(float d, float q, float& A, float& B, float& C);

};//CoordinateTrasformation