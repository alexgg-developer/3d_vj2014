#include "cHud.hpp"
#include <sstream>
#include <SDL.h>

bool Hud::init()
{
  bool success = false;

  if (mText.loadFont("./fonts/BALLOON1.ttf", 30) && mText.init()) {
    std::stringstream text;
    text << "LIFE: " << mLife << " MONEY: " << mMoney << " TOTAL WAVES: " << mTotalWaves << " PRESENT WAVE: " << mPresentWave;
    //text << "LIFE";
    SDL_Color color = { 255, 0, 0 };
    mText.loadText(text.str(), color, HIGH);
    mText.setPosition(vec3(0.0f, 0.75f, 0));
    mText.setScale(glm::vec3(1.f, 0.15f, 0.15f));
    success = true;
  }
  
  return success;
}

void Hud::update(uint life, uint money, uint totalWaves, uint presentWave)
{
  mLife = life;
  mMoney = money;
  mTotalWaves = totalWaves;
  mPresentWave = presentWave;
  std::stringstream text;
  text << "LIFE: " << mLife << "    MONEY: " << mMoney << "    TOTAL WAVES: " << mTotalWaves << "    PRESENT WAVE: " << mPresentWave;
  SDL_Color color = { 255, 0, 0 };
  mText.loadText(text.str() , color, HIGH);
}

void Hud::draw()
{
  mText.draw();
}