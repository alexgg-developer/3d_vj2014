#include "cTimer.hpp"

Timer::Timer() : mStartTime(0), mStarted(false), mPaused(false), mPausedTime(0), mLastTime(0)
{}

void Timer::start()
{
  mStartTime  = SDL_GetTicks();
  mLastTime  = SDL_GetTicks();
  mStarted    = true;
  mPaused     = false;
  mPausedTime = 0;
}

void Timer::restart()
{
  mStartTime  = SDL_GetTicks();
  mLastTime   = SDL_GetTicks();
  mStarted    = true;
  mPaused     = false;
  mPausedTime = 0;
}

float Timer::getDeltaTime()
{
  Uint32 deltaMilliseconds = SDL_GetTicks() - mLastTime;
  mLastTime = SDL_GetTicks();

  return static_cast<float>(deltaMilliseconds) / 1000.0f;
}

Uint32 Timer::getTimeElapsed() const
{
  Uint32 time;

  if(isStarted()) {
    if(mPaused) {
      time = mPausedTime;
    }
    else {
      time = SDL_GetTicks() - mStartTime;
    }
  }

  return time;
}

void Timer::pause()
{
  if( isStarted() && !mPaused ) {
    mPaused     = true;
    mPausedTime = SDL_GetTicks() - mStartTime;
    mStartTime  = 0;
  }
}

void Timer::resume()
{
  if( isStarted() && mPaused ) {
    mPaused      = false;
    mStartTime   = SDL_GetTicks() - mPausedTime;
    mPausedTime  = 0;
  }
}

bool Timer::isPaused() const
{
  return mPaused && mStarted;
}

bool Timer::isStarted() const
{
  return mStarted;
}
