#pragma once
#include "SDL.h"
#include "TypesDefined.hpp"
#include "vec3.hpp"
#include <glm\glm.hpp>

class Input
{
public:
  enum keysDefined{KUP, KDOWN, KRIGHT, KLEFT, KW, KA, KS, KD, KESC, nkeys};
  enum buttonsDefined{BLEFT, BMIDDLE, BRIGHT, nButtons};

  Input();
  void read(SDL_Event const & event);
  void readWithScanCode(SDL_Event const & event);
  bool check(unsigned int const key) const;
  bool checkPressed(unsigned int key);
  bool checkReleased(unsigned int key);
  bool checkMouse(uint button) const;
  bool checkMousePressed(uint button) const;
  bool checkMouseReleased(uint button) const;
  void UseMouseLastRelease(uint button);

  vec3 mPositionMousePressed, mPositionMouseRealased;
  glm::vec2 mMouseMovement;

private:
  //KEY_OFF: The key hasn't been pressed
  //KEY_ON: The key is being pressed
  //KEY_RELEASED: The key has just been released
  //KEY_PRESSED: The key has just been pressed
  enum keyStatus{ KEY_OFF, KEY_ON, KEY_RELEASED, KEY_PRESSED };
  unsigned int keys[nkeys];
  uint mouse[nButtons];

};
