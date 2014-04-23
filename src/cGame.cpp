#include "cGame.hpp"
#include "cTexture.hpp"

Game::Game()
{
  mScreenSurface = NULL;
  mWindow = NULL;
}

int Game::init()
{
  int error = initSDL();

  return error;
}

int Game::initSDL()
{
  int error = 0;
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    error = 1;
  }
  else
  {
    mWindow = SDL_CreateWindow( "SDL Base for Gaem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    if( mWindow == NULL )
    {
      std::cout <<  "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      error = 2;
    }
    else
    {
      mScreenSurface = SDL_GetWindowSurface( mWindow );

    }
  }

  return error;
}

int Game::quit()
{
  SDL_DestroyWindow( mWindow );
  SDL_Quit();

  return 0;
}

int Game::main()
{
  int error = init();
  Texture t = Texture();

  if(t.load("../resources/img/panda.bmp")) {
    SDL_BlitSurface( t.mTextureSurface, NULL, mScreenSurface, NULL );
  }

  SDL_UpdateWindowSurface( mWindow );
  SDL_Delay( 2000 );

  error = quit();

  return error;
}
