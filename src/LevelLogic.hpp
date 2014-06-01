#pragma once
#include <tuple>
#include <list>
#include <memory>
#include "Level.hpp"
#include "Turret.hpp"
#include "Enemy.hpp"
#include "Avalancha.hpp"
#include "Defensor.hpp"
#include "cSound.hpp"

struct Defensor;
struct LevelLogic {
  /// @param aLevel not owning pointer
  LevelLogic(Level const*const aLevel, Defensor *const aDefensor);
  ~LevelLogic();

  ///Initializes at specified time point
  void init(float const time_ms);
  ///Advances time from init_time_ms by dt_ms
  bool advanceTime(float const init_time_ms, float const dt_ms, std::vector<Enemy> const& availableEnemies, std::vector<Weapon> const& availableWeapons);
  void Render() const;
  
  void spawnsEnemy (EnemyLogic const& el);
  void spawnsTurret(TurretLogic&& el, float const build_init_time);
  MapLogic const& getMap() { return mMap; }

  bool has_ended(float const time_ms) const;
  bool user_won(float const time_ms) const;
  unsigned int how_much_waves() const;
  unsigned int actual_wave(float const time_ms) const;
protected:
  Level const*const mLevel;
  Defensor *const mDefensor;
  MapLogic mMap;
  Sound mBuyTurret;

  ///Turrets: alive and being constructed
  std::vector<std::unique_ptr<TurretLogic>> mAliveTurrets;
  std::vector<std::tuple<TurretLogic, float>> mBuildingTurrets; //Relates turrets and miliseconds when it was created

  ///Active enemies
  std::vector<std::unique_ptr<EnemyLogic>> mEnemies;
  ///Path that the enemies should go on
  std::vector<PathLogic> mPaths;
  std::vector<AvalanchaLogic> mAvalanchas;
};
