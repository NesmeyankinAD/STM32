#include "MaxMinModulation.h"

void MaxMinModulation::execute(std::vector<float> phases, float& A, float&B, float&C)
{
  float min = *std::min_element(begin(phases), end(phases));
  float max = *std::max_element(begin(phases), end(phases));

  A = (coef * phases[0]) - 0.5 * (min + max);
  B = (coef * phases[1]) - 0.5 * (min + max);
  C = (coef * phases[2]) - 0.5 * (min + max);
}