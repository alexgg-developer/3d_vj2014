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

  bool is_level_active() const;
  //has the level ended? Post: There are no enemies alive nor will be if we continue playing indefinitely. 
  //It makes no sense if there is no active level
  bool has_ended_level(float const time_ms) const;
  //has the user won the level? Post: Defensor is alive and has_level_ended
  //It makes no sense if there is no active level
  bool user_won(float const time_ms) const;
  //starts next level inmediatly. It will render and advance logic every frame from now on.
  void next_level();
  ///Deletes active level and stops from rendering nor updating anything
  void stop();
protected:
  std::vector<Weapon> mWeapons;
  std::vector<Enemy> mEnemies;
  std::vector<Turret> mTurrets;
  std::vector<Level> mLevels;

  LevelLogic *mActiveLevel=nullptr;//We can only have one active Level within one LevelManager
  Defensor mDefensor{100,100}; //The same player passes through all the levels. We can restore it's health between levels
};
