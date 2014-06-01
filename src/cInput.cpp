#include "cInput.hpp"
#include <iostream>

Input::Input() : mPositionMousePressed(vec3(0,0)), mPositionMouseRealased(vec3(0,0))
{
  for(uint i = 0; i < nkeys; ++i) keys[i] = KEY_OFF;
  for(uint i = 0; i < nButtons; ++i) mouse[i] = KEY_OFF;
}

void Input::read(SDL_Event const & event)
{
  //Keys RELEASED become OFF
  //Keys PRESSED become ON
  //It is in case that a key stay pressed it doesn't poll and event so you have to be aware of it
  for (uint i = 0; i < nkeys; ++i) keys[i] = keys[i] & 0x0001;
  switch(event.type) {
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
        case SDLK_UP:
          keys[KUP] = KEY_PRESSED;
          break;
        case SDLK_DOWN:
          keys[KDOWN] = KEY_PRESSED;
          break;
        case SDLK_RIGHT:
          keys[KRIGHT] = KEY_PRESSED;
          break;
        case SDLK_LEFT:
          keys[KLEFT] = KEY_PRESSED;
          break;
        case SDLK_w:
          keys[KW] = KEY_PRESSED;
          break;
        case SDLK_a:
          keys[KA] = KEY_PRESSED;
          break;
        case SDLK_s:
          keys[KS] = KEY_PRESSED;
          break;
        case SDLK_d:
          keys[KD] = KEY_PRESSED;
          break;
        case SDLK_ESCAPE:
          keys[KESC] = KEY_PRESSED;
          break;
      }
      break;
    case SDL_KEYUP:
      switch(event.key.keysym.sym) {
        case SDLK_UP:
          keys[KUP] = KEY_RELEASED;
          break;
        case SDLK_DOWN:
          keys[KDOWN] = KEY_RELEASED;
          break;
        case SDLK_RIGHT:
          keys[KRIGHT] = KEY_RELEASED;
          break;
        case SDLK_LEFT:
          keys[KLEFT] = KEY_RELEASED;
          break;
        case SDLK_w:
          keys[KW] = KEY_RELEASED;
          break;
        case SDLK_a:
          keys[KA] = KEY_RELEASED;
          break;
        case SDLK_s:
          keys[KS] = KEY_RELEASED;
          break;
        case SDLK_d:
          keys[KD] = KEY_RELEASED;
          break;
        case SDLK_ESCAPE:
          keys[KESC] = KEY_RELEASED;
          break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      switch(event.button.button) {
        case SDL_BUTTON_LEFT: {
          mouse[BLEFT] = KEY_PRESSED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMousePressed.x = static_cast<float>(x);
          mPositionMousePressed.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_MIDDLE: {
          mouse[BMIDDLE] = KEY_PRESSED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMousePressed.x = static_cast<float>(x);
          mPositionMousePressed.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_RIGHT: {
          mouse[BRIGHT] = KEY_PRESSED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMousePressed.x = static_cast<float>(x);
          mPositionMousePressed.y = static_cast<float>(y);
        }
        break;
        default:
          std::cout << "Event: " << static_cast<int>(event.button.button) << std::endl;
        break;
      }
    break;
    case SDL_MOUSEBUTTONUP:
      switch(event.button.button) {
        case SDL_BUTTON_LEFT: {
          mouse[BLEFT] = KEY_RELEASED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMouseRealased.x = static_cast<float>(x);
          mPositionMouseRealased.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_MIDDLE: {
          mouse[BMIDDLE] = KEY_RELEASED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMouseRealased.x = static_cast<float>(x);
          mPositionMouseRealased.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_RIGHT: {
          mouse[BRIGHT] = KEY_RELEASED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMouseRealased.x = static_cast<float>(x);
          mPositionMouseRealased.y = static_cast<float>(y);
        }
        break;
        default:
          std::cout << "Event: " << static_cast<int>(event.button.button) << std::endl;
        break;
      }
    break;
    case SDL_QUIT:
      keys[KESC] = KEY_ON;
      break;
  }
}


bool Input::check(unsigned int const key) const
{
  return keys[key] == KEY_ON || keys[key] == KEY_PRESSED;
}

bool Input::checkPressed(unsigned int key)
{
  return keys[key] == KEY_PRESSED;
}

bool Input::checkReleased(unsigned int key)
{
  return keys[key] == KEY_RELEASED;
}

bool Input::checkMouse(uint button) const
{
  return mouse[button] == KEY_ON || mouse[button] == KEY_PRESSED;
}

bool Input::checkMousePressed(uint button) const
{
  return mouse[button] == KEY_PRESSED;
}

bool Input::checkMouseReleased(uint button) const
{
  return mouse[button] == KEY_RELEASED;
}
void Input::UseMouseLastRelease(uint button) {
  assert(mouse[button] == KEY_RELEASED);
  mouse[button] = KEY_OFF;
}


void Input::readWithScanCode(SDL_Event const & event)
{
  //Keys RELEASED become OFF
  //Keys PRESSED become ON
  //It is in case that a key stay pressed it doesn't poll and event so you have to be aware of it
  for (uint i = 0; i < nkeys; ++i) keys[i] = keys[i] & 0x0001;
  //update actual mouse position
  int x, y;
  SDL_GetMouseState(&x, &y);
  mPositionMouse.x = static_cast<float>(x);
  mPositionMouse.y = static_cast<float>(y);
  switch(event.type) {
    case SDL_KEYDOWN:
      switch(event.key.keysym.scancode) {
        case SDL_SCANCODE_UP:
          keys[KUP] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_DOWN:
          keys[KDOWN] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_RIGHT:
          keys[KRIGHT] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_LEFT:
          keys[KLEFT] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_W:
          keys[KW] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_A:
          keys[KA] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_S:
          keys[KS] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_D:
          keys[KD] = KEY_PRESSED;
          break;
        case SDL_SCANCODE_ESCAPE:
          keys[KESC] = KEY_PRESSED;
          break;
      }
        default:
        break;
      break;
    case SDL_KEYUP:
      switch(event.key.keysym.scancode) {
        case SDL_SCANCODE_UP:
          keys[KUP] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_DOWN:
          keys[KDOWN] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_RIGHT:
          keys[KRIGHT] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_LEFT:
          keys[KLEFT] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_W:
          keys[KW] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_A:
          keys[KA] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_S:
          keys[KS] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_D:
          keys[KD] = KEY_RELEASED;
          break;
        case SDL_SCANCODE_ESCAPE:
          keys[KESC] = KEY_RELEASED;
          break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      switch(event.button.button) {
        case SDL_BUTTON_LEFT: {
          mouse[BLEFT] = KEY_PRESSED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMousePressed.x = static_cast<float>(x);
          mPositionMousePressed.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_MIDDLE: {
          mouse[BMIDDLE] = KEY_PRESSED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMousePressed.x = static_cast<float>(x);
          mPositionMousePressed.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_RIGHT: {
          mouse[BRIGHT] = KEY_PRESSED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMousePressed.x = static_cast<float>(x);
          mPositionMousePressed.y = static_cast<float>(y);
        }
        break;
        default:
          std::cout << "Event: " << static_cast<int>(event.button.button) << std::endl;
        break;
      }
    break;
    case SDL_MOUSEBUTTONUP:
      switch(event.button.button) {
        case SDL_BUTTON_LEFT: {
          mouse[BLEFT] = KEY_RELEASED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMouseRealased.x = static_cast<float>(x);
          mPositionMouseRealased.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_MIDDLE: {
          mouse[BMIDDLE] = KEY_RELEASED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMouseRealased.x = static_cast<float>(x);
          mPositionMouseRealased.y = static_cast<float>(y);
        }
        break;
        case SDL_BUTTON_RIGHT: {
          mouse[BRIGHT] = KEY_RELEASED;
          int x, y;
          SDL_GetMouseState(&x, &y);
          mPositionMouseRealased.x = static_cast<float>(x);
          mPositionMouseRealased.y = static_cast<float>(y);
        }
        break;
        default:
          std::cout << "Event: " << static_cast<int>(event.button.button) << std::endl;
        break;
      }

    case SDL_MOUSEMOTION: {
      switch (event.button.button) {
        case SDL_BUTTON_LEFT: {
          if (checkMouse(BLEFT)) {
            //std::cout << "X: " << x << " Y: " << y << std::endl;
            //SDL_GetRelativeMouseState(&x, &y);
            //std::cout << "XREL: " << event.motion.xrel << " YREL: " << event.motion.yrel << std::endl;
            //std::cout << "XREL2: " << x << " YREL2: " << y << std::endl;
            mMouseMovement = glm::vec2(event.motion.xrel, event.motion.yrel);
          }
        }
        break;
      }
    }
    break;
    case SDL_QUIT:
      keys[KESC] = KEY_ON;
      break;
  }

}


