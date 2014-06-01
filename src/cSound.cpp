#include "cSound.hpp"

Sound::Sound(): mSound(nullptr)
{
}

Sound::~Sound()
{
  //free();
}
#include <cassert>
void Sound::load(std::string path)
{
  free();
  mSound = Mix_LoadWAV( path.c_str() );
  if( mSound == NULL ) {
      std::cout << "Failed to load scratch sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
      assert(0);
  }
}

void Sound::free()
{
  if (mSound != nullptr) {
    Mix_FreeChunk(mSound);
  }
}

void Sound::play() const
{
  Mix_PlayChannel( -1, mSound, 0 );
}
