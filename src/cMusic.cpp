#include "cMusic.hpp"

Music::Music(): mMusic(nullptr)
{
}

Music::~Music()
{
  free();
}

void Music::load(std::string path)
{
  free();
  mMusic = Mix_LoadMUS( path.c_str() );
  if( mMusic == nullptr ) {
      std::cout << "Failed to load beat music! SDL_mixer Error:" << Mix_GetError() << std::endl;
  }
}

void Music::free()
{
  //Mix_FreeMusic( mMusic );
}

void Music::play(bool loop)
{
  if( Mix_PlayingMusic() == 0 ) {
      if(loop) Mix_PlayMusic( mMusic, -1 );
      else Mix_PlayMusic( mMusic, 0 );
  }
}

void Music::pause()
{
  Mix_PauseMusic();
}

void Music::resume()
{
  Mix_ResumeMusic();
}

void Music::stop()
{
  Mix_HaltMusic();
}

bool Music::isItPaused()
{
  return ( Mix_PausedMusic() == 1 );
}

bool Music::isItPlaying()
{
  return ( Mix_PlayingMusic() == 1 );
}
