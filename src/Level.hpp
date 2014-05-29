#pragma once
#include <string>
#include "Map.hpp"
#include "Avalancha.hpp"

struct LevelLogic;
struct Level {
  Level();
  ~Level();

  bool Load(std::string aFilename);

protected:
  Map mMap;
  std::vector<Path> mAssociatedPaths;
  std::vector<Avalancha*> mAvalanchas;
  friend struct LevelPlayer;
};

#include "Turret.hpp"
#include "Enemy.hpp"
#include <tuple>
struct LevelLogic {
  /// @param aLevel not owning pointer
  LevelLogic(Level const*const aLevel);
  ~LevelLogic();

  ///Initializes at specified time point
  void init(float const time_ms);
  ///Advances time from init_time_ms by dt_ms
  bool advanceTime(float const init_time_ms, float const dt_ms);
  void Render() const;

protected:
  Level const*const mLevel;

  ///Turrets: alive and being constructed
  std::vector<TurretLogic*> mAliveTurrets;
  std::vector<std::tuple<TurretLogic*, float>> mBuildingTurrets; //Relates turrets and miliseconds when it was created

  ///Active enemies
  std::vector<EnemyLogic> mEnemies;
};