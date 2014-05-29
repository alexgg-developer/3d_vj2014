#pragma once
#include <string>
#include "pugixml.hpp"
#include "glm/glm.hpp"

struct WeaponLogic;
struct Weapon {
  Weapon();
  ~Weapon();

  bool Load(pugi::xml_node aFilename);

  float miliseconds_per_bullet() const;
  float get_radius_tiles() const { return mRadiusTiles; }
protected:
  float mBulletsPerMinute=1000;
  float mDamage=1;
  float mRadiusTiles=1.5;
  enum SpecialEffect { NONE, ICE, BURN} mSpecialEffect;
  friend struct WeaponLogic;
};

struct EnemyLogic;
struct WeaponLogic {
  /// @param aLevel not owning pointer
  WeaponLogic(Weapon const*const aLevel);
  ~WeaponLogic();

  bool CanHit(float const time_ms);
  void Attack(EnemyLogic*const en, float const timeMS);
  void init(const float timeMS);

protected:
  Weapon const* mWeapon;
  float mLastShotMS=0;
};