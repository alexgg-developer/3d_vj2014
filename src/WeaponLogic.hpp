#pragma once
#include <string>

struct Weapon;
struct EnemyLogic;
struct WeaponLogic {
  /// @param aLevel not owning pointer
  WeaponLogic(Weapon const*const aLevel);
  ~WeaponLogic();

  bool CanHit(float const time_ms);
  void Attack(EnemyLogic*const en, float const timeMS);
  void init(const float timeMS);
  /// warning: use it only for enemies. Turret weapons may have additional effects
  float getDamage() const;
protected:
  Weapon const* mWeapon;
  float mLastShotMS=0;
};
