#include "LevelManager.hpp"

LevelManager::LevelManager() {}
LevelManager::~LevelManager() {
  if(mActiveLevel!=nullptr) delete mActiveLevel;}
void LevelManager::receive_input(float const end_frame_t, Input& in, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix) {
  if (mActiveLevel!=nullptr && mLevelInOut==NONE)
    mDefensor.receive_input(end_frame_t, in, *mActiveLevel, aProjectionMatrix, aMVMatrix, mTurrets, mWeapons);
}

bool LevelManager::is_level_active() const {
  return mActiveLevel!=nullptr && mLevelInOut!=NONE; }
bool LevelManager::has_ended_level(float const time_ms) const {
  return is_level_active() && mActiveLevel->has_ended(time_ms);
}
bool LevelManager::user_won(float const time_ms) const {
  return is_level_active() && mActiveLevel->user_won(time_ms);
}
bool LevelManager::has_next_level() const { 
  return mActiveLevelIndex+1<mLevels.size();
}
void LevelManager::reset_level(float const time_ms) {
  mDefensor.restoreMoney(mDefensorMoneyLastLevel);
  change_to_level(mActiveLevelIndex, time_ms);
}
void LevelManager::change_to_level(unsigned const int level, float const time_ms) {
  mActiveLevelIndex = level;
  if (mActiveLevel != nullptr)
    mLevelInOut = OUT;
  else {
    change_to_level_inmediate(mActiveLevelIndex, time_ms);
    mLevelInOut = IN;
  }
  mStartedInOutMS= time_ms;
}
void LevelManager::change_to_level_inmediate(unsigned const int level, float const time_ms) {
  mActiveLevelIndex = level;
  if(mActiveLevel!=nullptr) delete mActiveLevel;
  mActiveLevel = new LevelLogic(&mLevels[mActiveLevelIndex], &mDefensor);
  mActiveLevel->init(time_ms);
  mDefensor.restoreLife();
  mDefensorMoneyLastLevel = mDefensor.getMoney();
}
void LevelManager::next_level(float const time_ms) {
//TODO: Code correctly
  mActiveLevelIndex++;
  change_to_level(mActiveLevelIndex, time_ms);
}
void LevelManager::stop() {
//TODO: Animation for stoping. Maybe stop time and scale down everything?
  if(mActiveLevel!=nullptr) //I know this check is redundant, but it feels to be the RIGHT WAY
    delete mActiveLevel;
  mActiveLevel = nullptr;
}

bool LevelManager::init(float const te_ms) {
  assert(mActiveLevel);
  mActiveLevel->init(te_ms);
  mDefensor.init();
  return true;
}

bool LevelManager::advance_time(float const init_time_ms, float const dt_ms) {
  if(mActiveLevel && mLevelInOut==NONE)
    mActiveLevel->advanceTime(init_time_ms, dt_ms, mEnemies, mWeapons);

  if(mLevelInOut!=NONE) {
    if(!is_active_levelinout(init_time_ms)) {
      if(mLevelInOut==IN) {
        //change_to_level_inmediate(mActiveLevelIndex, init_time_ms);
        mActiveLevel->init(init_time_ms + dt_ms);
        mLevelInOut=NONE;
      } else {
        change_to_level_inmediate(mActiveLevelIndex, init_time_ms);
        mLevelInOut=IN;
        mStartedInOutMS=init_time_ms;
      }
    }
  }
  return true;
}
bool LevelManager::load() {
  //Load weapons
  loadWeapons("./levels/weapons.xml", std::back_inserter(mWeapons));

  //Load enemies
  loadEnemies("./levels/enemies.xml", std::back_inserter(mEnemies));
  
  //Load level
  {
    Level aLevel, aLevel1, aLevel2;
    bool const ret = aLevel.Load("./levels/level00.xml");
    assert(ret);
    mLevels.push_back(aLevel);
    bool const ret1 = aLevel1.Load("./levels/level01.xml");
    assert(ret1);
    mLevels.push_back(aLevel1);
    bool const ret2 = aLevel2.Load("./levels/level02.xml");
    assert(ret2);
    mLevels.push_back(aLevel2);
  }

  //Create turrets
  mTurrets.push_back(Turret(&mWeapons[0], 1000.0f, 10.0f)); //simple_gun
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  mTurrets.push_back(Turret(&mWeapons[1], 1000.0f, 20.0f)); //metralleta
  mTurrets.back().LoadModel("./objs/turret_2_separated4.obj");
  mTurrets.push_back(Turret(&mWeapons[2],10000.0f, 15.0f)); //shotgun
  mTurrets.back().LoadModel("./objs/turret_2_separated3.obj");
  mTurrets.push_back(Turret(&mWeapons[3], 3000.0f, 30.0f)); //ice_gun
  mTurrets.back().LoadModel("./objs/turret_2_separated2.obj");
  //loadTurrets("./levels/turret.xml", std::back_inserter(mEnemies));

  //if(mActiveLevel!=nullptr) delete mActiveLevel;
  //mActiveLevel = new LevelLogic(&mLevels[mActiveLevelIndex], &mDefensor);
  mDefensorMoneyLastLevel = mDefensor.getMoney();

  return true;
}
void LevelManager::render(float const time_ms) {
  glPushMatrix();
  if (mLevelInOut != NONE) {
    float scale;
    if (mLevelInOut == ScaleType::IN)
      scale = std::max(0.0f, time_ms - mStartedInOutMS) / mLengthInOutMS;
    else
      scale = std::max(0.0f, mLengthInOutMS-(time_ms - mStartedInOutMS)) / mLengthInOutMS;
    glScalef(scale, scale, scale);
  }
  if (mActiveLevel) mActiveLevel->Render();
  glPopMatrix();
}
