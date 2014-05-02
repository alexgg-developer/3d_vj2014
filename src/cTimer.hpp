#pragma once
#include "SDL.h"

class Timer
{
public:
  Timer();
  void start();
  float getDeltaTime() const;
  Uint32 getTimeElapsed() const;
  void restart();

private:
  Uint32 mStartTime;
};
