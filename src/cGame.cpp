#include "cGame.hpp"
#include "cTexture.hpp"

Game::Game()
{
  mRenderer = NULL;
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
      mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED );
      if( mRenderer == NULL ) {
          std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
          error = 3;
      }
      else {
        SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      }
    }
  }

  return error;
}

int Game::quit()
{
  SDL_DestroyWindow( mWindow );
  IMG_Quit();
  SDL_Quit();

  return 0;
}

int Game::main()
{
  int error = init();
  Texture t = Texture();

  if(!t.load("../resources/img/lena.png", mRenderer)) {
    return 4;
  }

  while(!mInput.check(Input::KESC)) {
    mInput.read();

    SDL_RenderClear( mRenderer );

    SDL_RenderCopy( mRenderer, t.mTexture, NULL, NULL );

    SDL_RenderPresent ( mRenderer );
  }

  error = quit();

  return error;
}
