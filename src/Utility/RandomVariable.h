#ifndef RANDOMVARIABLE_H_
#define RANDOMVARIABLE_H_

#include <stdint.h>
#include "stdlib.h"

static double lastValue = 0;

static double
GetRandomVariable (int seed, double maxValue)
{
  lastValue =  rand() * maxValue / RAND_MAX;
  return lastValue;
}


static double
GetRandomVariable (double maxValue)
{
  lastValue = rand() * maxValue / RAND_MAX;
  return lastValue;
}

#endif