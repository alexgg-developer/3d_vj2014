#include "LevelManager.hpp"

LevelManager::LevelManager() {}
LevelManager::~LevelManager() {
  if(mActiveLevel!=nullptr) delete mActiveLevel;}
void LevelManager::receive_input(Input& in, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix) {
  mDefensor.receive_input(in, *mActiveLevel, aProjectionMatrix, aMVMatrix, mTurrets, mWeapons);
}

bool LevelManager::init(float const te_ms) {
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
  bool const ret = aLevel.Load("./levels/level00.xml");
  assert(ret);

  //Create turrets
  mTurrets.push_back(Turret(&mWeapons[0])); //simple_gun
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  mTurrets.push_back(Turret(&mWeapons[1], 1.0f, 20.0f)); //metralleta
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  mTurrets.push_back(Turret(&mWeapons[2],10.0f, 15.0f)); //shotgun
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  mTurrets.push_back(Turret(&mWeapons[3], 3.0f, 30.0f)); //ice_gun
  mTurrets.back().LoadModel("./objs/turret_2_separated.obj");
  //loadTurrets("./levels/turret.xml", std::back_inserter(mEnemies));

  if(mActiveLevel!=nullptr) delete mActiveLevel;
  mActiveLevel = new LevelLogic(&aLevel, &mDefensor);

  return true;
}
void LevelManager::render() {
  if(mActiveLevel) mActiveLevel->Render();
}
