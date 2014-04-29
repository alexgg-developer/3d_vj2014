#pragma once
#include "SDL_mixer.h"
#include <string>
#include <iostream>

class Music
{
public:
  Music();
  ~Music();
  void load(std::string path);
  void free();
  void play(bool loop);
  void pause();
  void resume();
  void stop();
  bool isItPaused();
  bool isItPlaying();
private:
  Mix_Music* mMusic;
};
