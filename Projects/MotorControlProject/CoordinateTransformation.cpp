#include "CoordinateTransformation.h"

  void ABC_ab_transform(float A, float B, float C, float& a, float& b)
  {
    a = A;
    b = (B - C) / SQRT_3; 
  }

  void ab_ABC_transform(float a, float b, float& A, float& B, float& C)  
  {
    A =  a;
    B =  0.5 * (-a + SQRT_3 * b);
    C = -0.5 * (a + SQRT_3 * b);
  }

  void ab_dq_transform(float angle, float a, float b, float& d, float& q)  
  {
    float _cos = cosf(angle);
    float _sin = sinf(angle);

    d =  a * _cos + b * _sin;
    q = -a * _sin + b * _cos;
  }

  void dq_ab_transform(float angle, float d, float q, float& a, float& b)  
  {
    float _cos = cosf(angle);
    float _sin = sinf(angle);

    a = d * _cos - q * _sin;
    b = d * _sin + q * _cos;
  }

  void ABC_dq_transform(float angle, float A, float B, float C, float& d, float& q)  
  {
    float _cos = cosf(angle);
    float _sin = sinf(angle);

    d =  A * _cos + ((B - C) / SQRT_3) * _sin;
    q = -A * _sin + ((B - C) / SQRT_3) * _cos;
  }

  void dq_ABC_transform(float angle, float d, float q, float& A, float& B, float& C)  
  {
    float _cos = cosf(angle);
    float _sin = sinf(angle);

    A =  d * _cos - q * _sin;
    B =  0.5 * (SQRT_3 * (d * _sin + q *_cos) - A);
    C = -0.5 * (SQRT_3 * (d * _sin + q *_cos) + A);
  }
