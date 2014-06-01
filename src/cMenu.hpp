#pragma once
#include "cText.hpp"
#include "cInput.hpp"
#include <SDL.h>
#include <utility>

class Menu
{
public:
  Menu();
  bool init();
  size_t logic(Input const &i);
  void draw() const;
private:
  Text mLevel1, mLevel2, mLevel3, mQuit, mFullScreen;
  SDL_Color mColorText, mColorOverText;
  std::pair<glm::vec2, glm::vec2> mLevel1Coord, mLevel2Coord, mLevel3Coord, mQuitCoord, mFullScreenCoord; //1 -> topleft 2->bottomRight
  bool collission(glm::vec2 const & topLeft, glm::vec2 const & bottomRight, glm::vec2 const & point);
  bool mOverOption;
};