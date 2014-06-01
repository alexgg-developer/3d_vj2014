#pragma once
#include "TypesDefined.hpp"
#include "cText.hpp"

class Hud
{
public:
  Hud() : mLife(0), mMoney(0), mTotalWaves(0), mPresentWave(0) {}
  bool init();
  void update(uint life, uint money, uint totalWaves, uint presentWave);
  void draw();

private:
  uint mLife, mMoney, mTotalWaves, mPresentWave;
  Text mText;
};