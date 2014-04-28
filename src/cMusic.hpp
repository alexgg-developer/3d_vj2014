#pragma once
#include <SDL_mixer.h>

class Music
{
public:
  Music();

private:
  Mix_Music* mMusic;
};
