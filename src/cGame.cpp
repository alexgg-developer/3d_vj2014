#include "cGame.hpp"
#include "cTexture.hpp"
#include "vec3.hpp"
#include "cAnimation2D.hpp"
#include "cText.hpp"
#include "TypesDefined.hpp"
#include "SDL_ttf.h"
#include <sstream>

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
      mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
      if( mRenderer == NULL ) {
          std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
          error = 3;
      }
      else {
        SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      }
    }
    if( TTF_Init() == -1 ) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        error = 4;
    }
  }

  return error;
}

int Game::quit()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow( mWindow );
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}

int Game::main()
{
  int error = init();
  Texture t = Texture();

  if(!t.load("../resources/img/foo_animation.png", mRenderer, vec3(0, 255, 255))) {
    return 4;
  }

  Animation2D a(t, 4, 8, mRenderer);
  a.setConstantSizeOfImage();
  uint frame = 0;
  Text text(mRenderer);
  Text textMED(mRenderer);
  Text textHIGH(mRenderer);
  text.loadFont("../resources/fonts/lazy.ttf", 28);
  textMED.loadFont("../resources/fonts/lazy.ttf", 28);
  textHIGH.loadFont("../resources/fonts/lazy.ttf", 28);
  SDL_Color textColor = { 255, 0, 0, 255 };
  text.loadText("Such a text, much beauty LOW", textColor, LOW );
  textMED.loadText("Such a text, much beauty MED", textColor, MED );
  textHIGH.loadText("Such a text, much beauty HIGH", textColor, HIGH );
  text.setPosition(vec3(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 150));
  textMED.setPosition(vec3(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 100));
  textHIGH.setPosition(vec3(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 50));

  while(!mInput.check(Input::KESC)) {
    mInput.read();
    a.update(frame);


    SDL_RenderClear( mRenderer );
    a.draw();
    text.draw();
    textMED.draw();
    if(mInput.checkMouse(Input::BLEFT)) {
      std::stringstream ss;
      ss << "mPosition: " << mInput.mPositionMousePressed << " LEFT BUTTON " << std::endl;
      textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
    } else if (mInput.checkMouse(Input::BRIGHT)) {
      std::stringstream ss;
      ss << "mPosition: " << mInput.mPositionMousePressed << " BRIGHT BUTTON " << std::endl;
      textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
    } else if(mInput.checkMouse(Input::BMIDDLE)) {
      std::stringstream ss;
      ss << "mPosition: " << mInput.mPositionMousePressed << " BMIDDLE BUTTON " << std::endl;
      textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
    }
    textHIGH.draw();
    SDL_RenderPresent ( mRenderer );
    ++frame;
  }

  error = quit();

  return error;
}
