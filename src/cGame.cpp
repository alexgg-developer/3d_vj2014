#include "cGame.hpp"
#include "cTexture.hpp"
#include "vec3.hpp"

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

  if(!t.load("../resources/img/sprites.png", mRenderer, vec3(0, 255, 255))) {
    return 4;
  }

  while(!mInput.check(Input::KESC)) {
    mInput.read();

    SDL_RenderClear( mRenderer );
    SDL_Rect clipRed = { 0, 0, t.mWidth/2, t.mHeight/2 };
    SDL_Rect clipRedScreenPos = { 0, 0, t.mWidth/2, t.mHeight/2 };

    SDL_RenderCopy( mRenderer, t.mTexture, &clipRed, &clipRedScreenPos );

    SDL_Rect clipGreen = { t.mWidth/2, 0, t.mWidth/2, t.mHeight/2 };
    SDL_Rect clipGreenScreenPos = { WINDOW_WIDTH - t.mWidth/2, 0, t.mWidth/2, t.mHeight/2 };


    SDL_RenderCopy( mRenderer, t.mTexture, &clipGreen, &clipGreenScreenPos );

    SDL_Rect clipYellow = { 0, t.mHeight/2, t.mWidth/2, t.mHeight/2 };
    SDL_Rect clipYellowScreenPos = { 0, WINDOW_HEIGHT - t.mHeight/2, t.mWidth/2, t.mHeight/2 };


    SDL_RenderCopy( mRenderer, t.mTexture, &clipYellow, &clipYellowScreenPos );

    SDL_Rect clipBlue = { t.mWidth/2, t.mHeight/2, t.mWidth/2, t.mHeight/2 };
    SDL_Rect clipBlueScreenPos = { WINDOW_WIDTH - t.mWidth/2, WINDOW_HEIGHT - t.mHeight/2, t.mWidth/2, t.mHeight/2 };


    SDL_RenderCopy( mRenderer, t.mTexture, &clipBlue, &clipBlueScreenPos );


    SDL_RenderPresent ( mRenderer );
  }

  error = quit();

  return error;
}
