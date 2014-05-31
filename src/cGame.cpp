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
    //mWindow.switchFullScreen();

    mTimer.start();
    auto te_ms = mTimer.getTimeElapsed();
    lm.init(static_cast<float>(te_ms));
    mHud.update(10, 100, 2, 1);
    while(!mInput.check(Input::KESC)) {
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
      if(!mWindow.mMinimized) {
        float const dt = mTimer.getDeltaTime();
        float const te_ms = mTimer.getLastTimeMS();
        float const dt_ms = dt *1000.0f;
        logic(dt);

        //Render must be first
        
        mRenderer.render();
        mHud.draw();
        lm.render();

        lm.receive_input(te_ms+dt_ms, mInput, mRenderer.getProjMatrix(), mRenderer.mCamera.getViewMatrix());
        lm.advance_time(te_ms, dt_ms);

        SDL_GL_SwapWindow( mWindow.mWindow );
      }
    }

    error = quit();
  }
  else {
    std::cout << "Error initiating things" << std::endl;
    quit();
  }

  return error;
}

void Game::logic(float const dt)
{
  if (mInput.check(Input::KLEFT)) {
    mRenderer.mCamera.pan(glm::vec3(1, 0, -1), dt);
  }
  if (mInput.check(Input::KRIGHT)) {
    mRenderer.mCamera.pan(glm::vec3(-1, 0, 1), dt);
  }
  if (mInput.check(Input::KDOWN)) {
    mRenderer.mCamera.pan(glm::vec3(-1, 0, -1), dt);
  }
  if (mInput.check(Input::KUP)) {
    mRenderer.mCamera.pan(glm::vec3(1, 0, 1), dt);
  }
}

