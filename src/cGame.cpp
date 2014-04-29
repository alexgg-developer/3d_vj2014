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
    Text textHIGH(mRenderer);
    textHIGH.loadFont("../resources/fonts/lazy.ttf", 28);
    SDL_Color textColor = { 255, 0, 0, 255 };
    textHIGH.loadText("Such a text, much beauty HIGH", textColor, HIGH );
    textHIGH.setPosition(vec3(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 50));

    Music m;
    Sound s,s1,s2;
    m.load("../resources/music/partedelganado.mp3");
    s.load("../resources/sounds/high.wav");
    s1.load("../resources/sounds/medium.wav");
    s2.load("../resources/sounds/scratch.wav");

    while(!mInput.check(Input::KESC)) {
      mInput.readWithScanCode();

      SDL_RenderClear( mRenderer );
      if(mInput.checkMouse(Input::BLEFT)) {
        std::stringstream ss;
        ss << " PLAYING MUSIC" << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
        m.play(true);
      } else if (mInput.checkMouse(Input::BRIGHT)) {
        std::stringstream ss;
        ss << "MUSIC PAUSED" << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
        m.pause();
      } else if(mInput.checkMouse(Input::BMIDDLE)) {
        std::stringstream ss;
        ss << "MUSIC STOPPED " << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
        m.stop();
      } else if(mInput.check(Input::KA)) {
        std::stringstream ss;
        ss << "ISITPLAYING? " << m.isItPlaying() << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
      } else if(mInput.check(Input::KS)) {
        std::stringstream ss;
        ss << "ISITPLAUSED? " << m.isItPaused() << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
      }else if(mInput.check(Input::KD)) {
        std::stringstream ss;
        ss << "RESUME" << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
        m.resume();
      }else if(mInput.check(Input::KLEFT)) {
        std::stringstream ss;
        ss << "High sound" << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
        s.play();
      }else if(mInput.check(Input::KDOWN)) {
        std::stringstream ss;
        ss << "Medium sound" << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
        s1.play();
      }else if(mInput.check(Input::KRIGHT)) {
        std::stringstream ss;
        ss << "Scratch sound" << std::endl;
        textHIGH.loadText(ss.str().c_str(), textColor, HIGH );
        s2.play();
      }

      textHIGH.draw();
      SDL_RenderPresent ( mRenderer );
      ++frame;
    }
    m.free();
    s.free();
    s1.free();
    s2.free();
    textHIGH.free();
    error = quit();
  }
  else quit();

  return error;
}
