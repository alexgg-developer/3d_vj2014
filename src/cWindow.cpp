#include "cWindow.hpp"
#include <iostream>

Window::Window(): mWidth(DEFAULT_WINDOW_WIDTH), mHeight(DEFAULT_WINDOW_HEIGHT), mWindow(nullptr), mMinimized(false), mFullScreen(false),
  mRenderer(nullptr)
{

}

int Window::init()
{
  int error = 0;
  mWindow = SDL_CreateWindow( "SDL Base for Gaem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if( mWindow == NULL ) {
    std::cout <<  "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    error = 2;
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

void Window::free()
{
  SDL_DestroyRenderer( mRenderer );
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
