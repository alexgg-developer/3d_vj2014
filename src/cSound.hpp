#pragma once
#include "SDL_mixer.h"
#include <string>
#include <iostream>

class Sound
{
public:
  Sound();
  ~Sound();
  void load(std::string path);
  void free();
  void play();

private:
  Mix_Chunk* mSound;
};
