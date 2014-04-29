#include "cSound.hpp"

Sound::Sound(): mSound(nullptr)
{
}

Sound::~Sound()
{
  //free();
}

void Sound::load(std::string path)
{
  free();
  mSound = Mix_LoadWAV( path.c_str() );
  if( mSound == NULL ) {
      std::cout << "Failed to load scratch sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
  }
}

void Sound::free()
{
  Mix_FreeChunk( mSound );
}

void Sound::play()
{
  Mix_PlayChannel( -1, mSound, 0 );
}
