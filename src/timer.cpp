#include <Arduino.h>
#include "timer.h"

Timer::Timer(
    unsigned long limit)
{
  this->limit = limit;
  this->resetTime = millis();
}

void Timer::reset()
{
  this->resetTime = millis();
}

bool Timer::elapsed()
{
  return millis() - this->resetTime > this->limit;
}
