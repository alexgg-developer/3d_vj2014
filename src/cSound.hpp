#pragma once
#include <SDL_mixer.h>

class Sound
{
public:
  Sound();

private:
  Mix_Chunk* mSound;
};
