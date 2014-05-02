#include "cTimer.hpp"

Timer::Timer() : mStartTime(0)
{}

void Timer::start()
{
  mStartTime = SDL_GetTicks();
}

void Timer::restart()
{
  start();
}

float Timer::getDeltaTime() const
{
  Uint32 elapsedMilliseconds = SDL_GetTicks() - mStartTime;

  return static_cast<float>(elapsedMilliseconds) / 1000.0f;
}

Uint32 Timer::getTimeElapsed() const
{
  return (SDL_GetTicks() - mStartTime);
}
