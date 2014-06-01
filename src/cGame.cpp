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
#include <glm\glm.hpp>
#include "cAssimpModel.h"
#include "Level.hpp"
#include "Defensor.hpp"

Game::Game()
{}

int Game::init()
{
  mInMenu = true;
  mMenuOption = 0;
  mQuitDone = false;
  mLevel = 0;
  int error = initSDL();
  if(error == 0) error = initGLEW();
  if(error == 0) error = initGL();

  if (!mMusic.load("./music/Sardonica_-_Through_to_Reality__Original_.mp3")) {
    error = 30;
  }
  mMusic.play(true);

  if (!mHud.init()) {
    error = 40;
  }

  if (!mMenu.init()){
    error = 50;
  }

  mWinSound.load("./sounds/you_win.wav");
  mLoseSound.load("./sounds/you_lose.wav");

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

  error = mWindow.init();

  if(error == 0) {
    if( TTF_Init() == -1 ) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        error = 4;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
       std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
       error = 6;
    }
  }

  return error;
}

int Game::initGLEW()
{
  int error = 0;
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK ) {
    std::cout << "Error initializing GLEW! " << glewGetErrorString( glewError ) << std::endl;
    error = 10;
  }

  return error;
}

int Game::initGL()
{
  int error = 0;
  if (!mRenderer.initGL(mWindow.mWidth, mWindow.mHeight) || !mRenderer.initApp()) {
    error = 20;
  }
  return error;
}

int Game::quit()
{
  mWindow.free();
  mRenderer.free();
  mMusic.free();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}

#include "LevelManager.hpp"
int Game::main() {
  int error = init();
  LevelManager lm;
  lm.load();
  if(!error) {
    uint frame = 0;
    mTimer.start();
    //lm.init(static_cast<float>(mTimer.getLastTimeMS()));
    while(!mInput.check(Input::KESC) && !mQuitDone) {
      //Prepare input
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if(event.type == SDL_WINDOWEVENT) {
          mWindow.handleEvent(event);
          if(mWindow.mMinimized) {
            mTimer.pause();
            mMusic.pause();
          }
          else {
            mTimer.resume();
            mMusic.resume();
          }
        }
        else {
          mInput.readWithScanCode(event);
        }
      }
      //Advance game
      if (!mWindow.mMinimized) {
        float const dt = mTimer.getDeltaTime();
        float const te_ms = mTimer.getLastTimeMS();
        float const dt_ms = dt *1000.0f;
        logic(dt, lm, te_ms);

        //Render must be first
        if (!mInMenu && !mQuitDone) {
          if (lm.is_level_active())
            mHud.update(static_cast<unsigned int>(std::max<float>(lm.get_life(), 0.0f)), static_cast<unsigned int>(lm.get_money()), lm.how_much_waves_in_actual_level(), lm.actual_wave_in_actual_leve(te_ms));
          mRenderer.render();
          mHud.draw();
          lm.render(te_ms);

          lm.receive_input(te_ms+dt_ms, mInput, mRenderer.getProjMatrix(), mRenderer.mCamera.getViewMatrix());
          lm.advance_time(te_ms, dt_ms);

          /// Treat end of levels, wins and loses
          if(lm.has_ended_level(te_ms+dt_ms) || lm.get_life()<=0.0f) {
            if(lm.user_won(te_ms+dt_ms)) {
              if(lm.has_next_level())
                lm.next_level(te_ms+dt_ms);
              else {
                std::cout << "You won the game" << std::endl;
                mInMenu = true;
                mWinSound.play();
                //exit(0);
              }
            } else {
              std::cout << "You lost the game" << std::endl;
              //TODO return to main menu or reset level
              lm.reset_level(te_ms+dt_ms);
              mLoseSound.play();
            }
          }
        }
        else if (!mQuitDone){
          mMenu.draw();
        }
        SDL_GL_SwapWindow( mWindow.mWindow );
      }
    }
    if (!mQuitDone) error = quit();
  }
  else {
    std::cout << "Error initiating things" << std::endl;
    quit();
  }

  return error;
}

void Game::logic(float const dt, LevelManager & lm, float const te)
{
  if (mInput.check(Input::KLEFT)) {
    mRenderer.mCamera.pan(glm::vec3(1, 0, 0), dt);
  }
  if (mInput.check(Input::KRIGHT)) {
    mRenderer.mCamera.pan(glm::vec3(-1, 0, 0), dt);
  }
  if (mInput.check(Input::KDOWN)) {
    mRenderer.mCamera.pan(glm::vec3(0, 0, -1), dt);
  }
  if (mInput.check(Input::KUP)) {
    mRenderer.mCamera.pan(glm::vec3(0, 0, 1), dt);
  }
  if (mInMenu) {
    mMenuOption = mMenu.logic(mInput);
    switch (mMenuOption) {
    case 1:
      mLevel = 1;
      lm.change_to_level(mLevel - 1, te);
      mInMenu = false;
      break;
    case 2:
      mLevel = 2;
      lm.change_to_level(mLevel - 1, te);
      mInMenu = false;
      break;
    case 3:
      mLevel = 3;
      lm.change_to_level(mLevel - 1, te);
      mInMenu = false;
      break;
    case 4:
      quit();
      mInMenu = false;
      mQuitDone = true;
      break;
    case 5:
      mWindow.switchFullScreen();
      break;
    default:
      //std::cout << "mMenuOption: " << mMenuOption << std::endl;
      break;
    }
  }

}

