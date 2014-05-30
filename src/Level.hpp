#pragma once
#include <string>
#include "Map.hpp"
#include "Avalancha.hpp"

struct Level {
  Level();
  ~Level();

  bool Load(std::string aFilename);

protected:
  Map mMap;
  std::vector<Path> mAssociatedPaths;
  std::vector<Avalancha*> mAvalanchas;
  friend struct LevelLogic;
};

#include "Turret.hpp"
#include "Enemy.hpp"
#include <tuple>
#include "Avalancha.hpp"
#include "Turret.hpp"
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
  void spawnsTurret(TurretLogic&& el);
  MapLogic const& getMap() { return mMap; }

protected:
  Level const*const mLevel;
  Defensor *const mDefensor;
  MapLogic mMap;

  ///Turrets: alive and being constructed
  std::vector<TurretLogic> mAliveTurrets;
  std::vector<std::tuple<TurretLogic, float>> mBuildingTurrets; //Relates turrets and miliseconds when it was created

  ///Active enemies
  std::vector<EnemyLogic> mEnemies;
  ///Path that the enemies should go on
  std::vector<PathLogic> mPaths;
  std::vector<AvalanchaLogic> mAvalanchas;
};