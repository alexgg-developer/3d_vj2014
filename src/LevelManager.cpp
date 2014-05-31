#include "LevelManager.hpp"

LevelManager::LevelManager() {}
LevelManager::~LevelManager() {
  if(mActiveLevel!=nullptr) delete mActiveLevel;}
void LevelManager::receive_input(float const end_frame_t, Input& in, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix) {
  mDefensor.receive_input(end_frame_t, in, *mActiveLevel, aProjectionMatrix, aMVMatrix, mTurrets, mWeapons);
}

bool LevelManager::is_level_active() const {
  return mActiveLevel!=nullptr; }
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
  mActiveLevelIndex--;
  mDefensor.restoreMoney(mDefensorMoneyLastLevel);
  next_level(time_ms);
}
void LevelManager::next_level(float const time_ms) {
//TODO: Code correctly
  mActiveLevelIndex++;
  if(mActiveLevel!=nullptr) delete mActiveLevel;
  mActiveLevel = new LevelLogic(&mLevels[mActiveLevelIndex], &mDefensor);
  mActiveLevel->init(time_ms);
  mDefensor.restoreLife();
  mDefensorMoneyLastLevel = mDefensor.getMoney();
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
  if(mActiveLevel)
    mActiveLevel->advanceTime(mDefensor, init_time_ms, dt_ms, mEnemies, mWeapons);
  return true;
}
bool LevelManager::load() {
  //Load weapons
  loadWeapons("./levels/weapons.xml", std::back_inserter(mWeapons));

  //Load enemies
  loadEnemies("./levels/enemies.xml", std::back_inserter(mEnemies));
  
  //Load level
  {
    Level aLevel;
    bool const ret = aLevel.Load("./levels/level00.xml");
    assert(ret);
    mLevels.push_back(aLevel);
  }

  //Create turrets
  mTurrets.push_back(Turret(&mWeapons[0], 1000.0f, 10.0f)); //simple_gun
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  mTurrets.push_back(Turret(&mWeapons[1], 1000.0f, 20.0f)); //metralleta
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  mTurrets.push_back(Turret(&mWeapons[2],10000.0f, 15.0f)); //shotgun
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  mTurrets.push_back(Turret(&mWeapons[3], 3000.0f, 30.0f)); //ice_gun
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  //loadTurrets("./levels/turret.xml", std::back_inserter(mEnemies));

  if(mActiveLevel!=nullptr) delete mActiveLevel;
  mActiveLevel = new LevelLogic(&mLevels[mActiveLevelIndex], &mDefensor);

  return true;
}
void LevelManager::render() {
  if(mActiveLevel) mActiveLevel->Render();
}
