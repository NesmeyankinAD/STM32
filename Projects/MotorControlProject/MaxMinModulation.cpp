#include "MaxMinModulation.h"

void MaxMinModulation::execute(std::vector<float> phases, float& A, float&B, float&C)
{
  float min = *std::min_element(begin(phases), end(phases));
  float max = *std::max_element(begin(phases), end(phases));

  A = (coef * phases[0]) - 0.5 * (min + max);
  B = (coef * phases[1]) - 0.5 * (min + max);
  C = (coef * phases[2]) - 0.5 * (min + max);
}

void MaxMinModulation::execute(std::vector<float> phases, std::vector<float> max_min)
{
  float min = *std::min_element(begin(phases), end(phases));
  float max = *std::max_element(begin(phases), end(phases));

  max_min[0] = (coef * phases[0]) - 0.5 * (min + max);
  max_min[1] = (coef * phases[1]) - 0.5 * (min + max);
  max_min[2] = (coef * phases[2]) - 0.5 * (min + max);
}

void MaxMinModulation::execute(float* phases, float* max_min)
{
  float min = *std::min_element(phases, phases+3);
  float max = *std::max_element(phases, phases+3);

  max_min[0] = (coef * phases[0]) - 0.5 * (min + max);
  max_min[1] = (coef * phases[1]) - 0.5 * (min + max);
  max_min[2] = (coef * phases[2]) - 0.5 * (min + max);
}