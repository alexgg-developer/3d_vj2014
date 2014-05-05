#include "cWindow.hpp"
#include "SDL_opengl.h"
#include <iostream>

Window::Window(): mWidth(DEFAULT_WINDOW_WIDTH), mHeight(DEFAULT_WINDOW_HEIGHT), mWindow(nullptr), mMinimized(false), mFullScreen(false),
  mRenderer(nullptr)
{

}

int Window::init()
{
  int error = 0;
  mWindow = SDL_CreateWindow( "SDL Base for Gaem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if( mWindow == nullptr ) {
    std::cout <<  "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    error = 2;
  }
  else {
    if(!createGLContext()) {
      error = 2;
    }
  }

  return error;
}

SDL_Renderer* Window::createRenderer()
{
  if(mRenderer == nullptr) {
    mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
  }

  return mRenderer;
}

bool Window::createGLContext()
{
  //Use OpenGL 3.1 core
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  mContext = SDL_GL_CreateContext( mWindow );
  if(mContext == nullptr) {
    std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
    return false;
  }
  else {
    if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
        std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
  }

  return true;
}

void Window::free()
{
  if(mRenderer != nullptr) SDL_DestroyRenderer( mRenderer );
  SDL_DestroyWindow( mWindow );
}

void Window::handleEvent(SDL_Event const & event)
{
  switch(event.window.event){
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      mWidth  = event.window.data1;
      mHeight = event.window.data2;
      SDL_RenderPresent( mRenderer );
    break;
    case SDL_WINDOWEVENT_EXPOSED:
      SDL_RenderPresent( mRenderer );
    break;
    case SDL_WINDOWEVENT_MAXIMIZED:
      mMinimized = false;
    break;
    case SDL_WINDOWEVENT_MINIMIZED:
      mMinimized = true;
    break;
    case SDL_WINDOWEVENT_RESTORED:
      mMinimized = false;
    break;
  }
}

void Window::switchFullScreen()
{
  if( mFullScreen ) {
      SDL_SetWindowFullscreen( mWindow, SDL_FALSE );
      mFullScreen = false;
  }
  else {
      SDL_SetWindowFullscreen( mWindow, SDL_TRUE );
      mFullScreen = true;
      mMinimized = false;
  }
}
