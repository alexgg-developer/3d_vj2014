#pragma once
#include <vector>
#include "Weapon.hpp"
#include "Enemy.hpp"
#include "Turret.hpp"
#include "Level.hpp"
#include "Defensor.hpp"

struct LevelManagerLogic;
struct Weapon;
struct LevelManager {
  LevelManager();
  ~LevelManager();
  
  bool load();
  bool init(float const te_ms);
  void render();

  ///Advances time from init_time_ms by dt_ms
  bool advance_time(float const init_time_ms, float const dt_ms);
  LevelLogic* getActiveLevel() { return mActiveLevel; }
  void receive_input(float const end_frame_t, Input& in, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix);
protected:
  std::vector<Weapon> mWeapons;
  std::vector<Enemy> mEnemies;
  std::vector<Turret> mTurrets;
  LevelLogic *mActiveLevel=nullptr;
  Level aLevel;
  Defensor mDefensor{100,100};
};
