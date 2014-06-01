#pragma once
#include <glm/glm.hpp>
#include "pugixml.hpp"
#include "WeaponLogic.hpp"
#include "Enemy.hpp"

struct Defensor;
struct EnemyLogic {
  /// @param aLevel not owning pointer
  EnemyLogic(Enemy const*const anEnemy, WeaponLogic&& awl);
  ~EnemyLogic();

  ///Initializes at specified time point
  void init(float const time_ms);
  ///Advances time
  //bool advanceTime(float const dt_ms);
  void ReceiveDamage(float const damage);
  void Attack(Defensor& df);

  bool hasDied() const {
    return mActualLife <= 0;
  }
  float life() const { return mActualLife; }

  void Render() const;
  glm::vec2 const& getPosition() const { return mPosition; }
  void setPosition(glm::vec2 const& p) { mPosition=p; }
  float getEnemyMonetaryValue() const { return mEnemy->mMonetaryValue; }

  /// Some weapons, like ICE, need to stop the time for the enemy.
  /// Every hit by an ICE weapon will add more time to stop, so we can stop enemies indefinitely.
  /// TODO: put some limit to stopped time
  void add_time_to_stop(float const dt_ms) {
   mTimeToStop+=dt_ms;}
  float time_stopped() const { return mTimeStopped; }
  float time_to_stop() const { return mTimeToStop; }
  void add_stopped_time(float const dt_ms) {
   mTimeStopped = std::min(dt_ms+mTimeStopped, mTimeToStop); }

  bool is_iced() const { return time_to_stop() > time_stopped();}
  bool mPathFinished = false;
  float getVelocity_tiles_per_ms() const {
    return mEnemy->mTilesPerMinute * 1.0f/60000.0f;
  }
protected:
  Enemy const*const mEnemy;
  WeaponLogic mWeaponLogic;
  float mActualLife;
  glm::vec2 mPosition{-1,-1};
  float mLastMovedMS=-1;
  float mTimeStopped=0;
  float mTimeToStop=0;
};
