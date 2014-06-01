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
  void render(float const time_ms);
  
  float get_money() const { return mDefensor.getMoney(); }  
  float get_life() const { return mDefensor.getLife(); }
  unsigned int how_much_waves_in_actual_level() const { return mActiveLevel->how_much_waves(); }
  unsigned int actual_wave_in_actual_leve(float const time_ms) const { return mActiveLevel->actual_wave(time_ms)+1; }

  ///Advances time from init_time_ms by dt_ms
  bool advance_time(float const init_time_ms, float const dt_ms);
  LevelLogic* getActiveLevel() { return mActiveLevel; }
  void receive_input(float const end_frame_t, Input& in, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix);

  bool has_next_level() const;
  bool is_level_active() const;
  //has the level ended? Post: There are no enemies alive nor will be if we continue playing indefinitely. 
  //It makes no sense if there is no active level
  bool has_ended_level(float const time_ms) const;
  //has the user won the level? Post: Defensor is alive and has_level_ended
  //It makes no sense if there is no active level
  bool user_won(float const time_ms) const;
  //starts next level inmediatly. It will render and advance logic every frame from now on.
  void next_level(float const time_ms);
  ///Deletes active level and stops from rendering nor updating anything
  void stop();
  void reset_level(float const time_ms);
  
  void change_to_level(unsigned const int level, float const time_ms);
  void change_to_level_inmediate(unsigned const int level, float const time_ms);
protected:
  std::vector<Weapon> mWeapons;
  std::vector<Enemy> mEnemies;
  std::vector<Turret> mTurrets;
  std::vector<Level> mLevels;

  LevelLogic *mActiveLevel=nullptr;//We can only have one active Level within one LevelManager
  Defensor mDefensor{100,1000}; //The same player passes through all the levels. We can restore it's health between levels
  float mDefensorMoneyLastLevel;
  std::size_t mActiveLevelIndex=0;


  /// scale in/out
  enum ScaleType{ NONE, IN, OUT} mLevelInOut;
  float mStartedInOutMS=0;
  float mLengthInOutMS=3000.0f;
  float EndInOut_ms() const { return mLengthInOutMS+mStartedInOutMS;}
  bool is_active_levelinout(float const time_ms) const { return EndInOut_ms()>time_ms;}
};
