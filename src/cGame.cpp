#include "cGame.hpp"
#include "cTexture.hpp"
#include "vec3.hpp"
#include "cAnimation2D.hpp"
#include "cText.hpp"
#include "TypesDefined.hpp"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "cMusic.hpp"
#include "cSound.hpp"
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
  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
  {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    error = 1;
  }
  else
  {
    mWindow = SDL_CreateWindow( "SDL Base for Gaem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    if( mWindow == NULL ) {
      std::cout <<  "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      error = 2;
    }
    else {
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
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" ) ) {
        std::cout << "Warning: Anisotropic texture filtering not enabled!" << std::endl;
        error = 5;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
       std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
       error = 6;
    }
  }

  return error;
}

int Game::quit()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow( mWindow );
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}

int Game::main()
{
  int error = init();

  if(!error) {
    uint frame = 0;
    Text textTime(mRenderer);
    Text textLoopTime(mRenderer);
    textTime.loadFont("../resources/fonts/lazy.ttf", 28);
    textLoopTime.loadFont("../resources/fonts/lazy.ttf", 28);
    SDL_Color textColor = { 255, 0, 0, 255 };
    textTime.loadText("Such a text, much beauty HIGH", textColor, HIGH );
    textTime.setPosition(vec3(0, 0));
    textLoopTime.loadText("Such a text, much beauty HIGH", textColor, HIGH );
    textLoopTime.setPosition(vec3(0, textTime.mHeight));


    Timer timer;
    timer.start();
    mTimer.start();
    while(!mInput.check(Input::KESC)) {
      mInput.readWithScanCode();
      SDL_RenderClear( mRenderer );
      std::stringstream time;
      time << " Loop time " << timer.getDeltaTime() << std::endl;
      textTime.loadText(time.str().c_str(), textColor, HIGH );

      std::stringstream ss;
      ss << "Time since beginning " << mTimer.getTimeElapsed() << std::endl;
      textLoopTime.loadText(ss.str().c_str(), textColor, HIGH );

      if(mInput.check(Input::KA)){
        if(!mTimer.isPaused()) mTimer.pause();
      } else if(mInput.check(Input::KS)){
        if(mTimer.isPaused()) mTimer.resume();
      }

      textTime.draw();
      textLoopTime.draw();
      SDL_RenderPresent ( mRenderer );
      ++frame;
    }


    textTime.free();
    textLoopTime.free();
    error = quit();
  }
  else quit();

  return error;
}

