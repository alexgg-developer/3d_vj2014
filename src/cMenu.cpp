#include "cMenu.hpp"
#include <sstream>

Menu::Menu()
{
  mColorText = { 255, 0, 0 };
  mColorOverText = { 0, 0, 255 };
  mOverOption = false;
}

bool Menu::init()
{
  bool success = false;
  if (mLevel1.loadFont("./fonts/BALLOON1.ttf", 30) && mLevel1.init()) {
    std::stringstream text;
    text << "LEVEL 1";
    mLevel1.loadText(text.str(), mColorText, HIGH);
    mLevel1.setPosition(vec3(0.0f, 0.4f, 0));
    mLevel1.setScale(glm::vec3(0.5f, 0.15f, 0.15f));
    mLevel1Coord = std::make_pair(glm::vec2(310, 165), glm::vec2(500, 193));
    success = true;
  }
  else { success = false; }

  if (mLevel2.loadFont("./fonts/BALLOON1.ttf", 30) && mLevel2.init()) {
    std::stringstream text;
    text << "LEVEL 2";
    mLevel2.loadText(text.str(), mColorText, HIGH);
    mLevel2.setPosition(vec3(0.0f, 0.2f, 0));
    mLevel2.setScale(glm::vec3(0.5f, 0.15f, 0.15f));
    mLevel2Coord = std::make_pair(glm::vec2(310, 224), glm::vec2(500, 255));
    success = true;
  }
  else { success = false; }

  if (mLevel3.loadFont("./fonts/BALLOON1.ttf", 30) && mLevel3.init()) {
    std::stringstream text;
    text << "LEVEL 3";
    mLevel3.loadText(text.str(), mColorText, HIGH);
    mLevel3.setPosition(vec3(0.0f, 0.0f, 0));
    mLevel3.setScale(glm::vec3(0.5f, 0.15f, 0.15f));
    mLevel3Coord = std::make_pair(glm::vec2(310, 280), glm::vec2(500, 315));
    success = true;
  }
  else { success = false; }

  if (mQuit.loadFont("./fonts/BALLOON1.ttf", 40) && mQuit.init()) {
    std::stringstream text;
    text << "QUIT";
    mQuit.loadText(text.str(), mColorText, HIGH);
    mQuit.setPosition(vec3(0.0f, -0.2f, 0));
    mQuit.setScale(glm::vec3(0.5f, 0.15f, 0.15f));
    mQuitCoord = std::make_pair(glm::vec2(310, 340), glm::vec2(500, 375));
    success = true;
  }
  else { success = false; }

  if (mFullScreen.loadFont("./fonts/BALLOON1.ttf", 15) && mFullScreen.init()) {
    std::stringstream text;
    text << "FULLSCREEN TOGGLE ON/OFF";
    mFullScreen.loadText(text.str(), mColorText, HIGH);
    mFullScreen.setPosition(vec3(0.0f, -0.75f, 0));
    mFullScreen.setScale(glm::vec3(0.50f, 0.15f, 0.15f));
    mFullScreenCoord = std::make_pair(glm::vec2(300, 500), glm::vec2(500, 540));
    success = true;
  }
  else { success = false; }


  return success;
}

bool Menu::collission(glm::vec2 const & topLeft, glm::vec2 const & bottomRight, glm::vec2 const & point)
{
  bool collision = false;
  bool horizontalCollision = (point.x >= topLeft.x && point.x <= bottomRight.x);
  bool verticalCollision = (point.y >= topLeft.y && point.y <= bottomRight.y);

  return horizontalCollision && verticalCollision;
}

size_t Menu::logic(Input const &i)
{
  size_t option = 0;
  glm::vec2 point = i.mPositionMouse;

  std::stringstream text1;
  text1 << "LEVEL 1";
  std::stringstream text2;
  text2 << "LEVEL 2";
  std::stringstream text3;
  text3 << "LEVEL 3";
  std::stringstream text4;
  text4 << "QUIT";
  std::stringstream text5;
  text5 << "FULLSCREEN TOGGLE ON/OFF";

  if (collission(mLevel1Coord.first, mLevel1Coord.second, point)) {
    mLevel1.loadText(text1.str(), mColorOverText, HIGH);
    mLevel2.loadText(text2.str(), mColorText, HIGH);
    mLevel3.loadText(text3.str(), mColorText, HIGH);
    mQuit.loadText(text4.str(), mColorText, HIGH);
    mFullScreen.loadText(text5.str(), mColorText, HIGH);
    if (i.checkMouse(Input::BLEFT)) {
      option = 1;
    }
  }
  else if (collission(mLevel2Coord.first, mLevel2Coord.second, point)){
    mLevel2.loadText(text2.str(), mColorOverText, HIGH);
    mLevel1.loadText(text1.str(), mColorText, HIGH);
    mLevel3.loadText(text3.str(), mColorText, HIGH);
    mQuit.loadText(text4.str(), mColorText, HIGH);
    mFullScreen.loadText(text5.str(), mColorText, HIGH);
    if (i.checkMouse(Input::BLEFT)) {
      option = 2;
    }
  }
  else if (collission(mLevel3Coord.first, mLevel3Coord.second, point)){
    mLevel3.loadText(text3.str(), mColorOverText, HIGH);
    mLevel1.loadText(text1.str(), mColorText, HIGH);
    mLevel2.loadText(text2.str(), mColorText, HIGH);
    mQuit.loadText(text4.str(), mColorText, HIGH);
    mFullScreen.loadText(text5.str(), mColorText, HIGH);
    if (i.checkMouse(Input::BLEFT)) {
      option = 3;
    }
  }
  else if (collission(mQuitCoord.first, mQuitCoord.second, point)){
    mQuit.loadText(text4.str(), mColorOverText, HIGH);
    mLevel1.loadText(text1.str(), mColorText, HIGH);
    mLevel2.loadText(text2.str(), mColorText, HIGH);
    mLevel3.loadText(text3.str(), mColorText, HIGH);
    mFullScreen.loadText(text5.str(), mColorText, HIGH);
    if (i.checkMouse(Input::BLEFT)) {
      option = 4;
    }
  }
  else if (collission(mFullScreenCoord.first, mFullScreenCoord.second, point)){
    mFullScreen.loadText(text5.str(), mColorOverText, HIGH);
    mLevel1.loadText(text1.str(), mColorText, HIGH);
    mLevel2.loadText(text2.str(), mColorText, HIGH);
    mLevel3.loadText(text3.str(), mColorText, HIGH);
    mQuit.loadText(text4.str(), mColorText, HIGH);
    if (i.checkMouse(Input::BLEFT)) {
      option = 5;
    }
  }
  else {
    mLevel1.loadText(text1.str(), mColorText, HIGH);
    mLevel2.loadText(text2.str(), mColorText, HIGH);
    mLevel3.loadText(text3.str(), mColorText, HIGH);
    mQuit.loadText(text4.str(), mColorText, HIGH);
    mFullScreen.loadText(text5.str(), mColorText, HIGH);
  }

  return option;
}

void Menu::draw() const
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mLevel1.draw();
  mLevel2.draw();
  mLevel3.draw();
  mQuit.draw();
  mFullScreen.draw();
}