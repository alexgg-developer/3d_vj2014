#pragma once
#include "SDL.h"

class Timer
{
public:
  Timer();
  void start();
  float getDeltaTime();
  Uint32 getTimeElapsed()const;
  void restart();
  void pause();
  void resume();
  bool isStarted() const;
  bool isPaused() const;

private:
  Uint32 mStartTime, mPausedTime, mLastTime;
  bool mStarted, mPaused;
};
