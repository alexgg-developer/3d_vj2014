#pragma once
#include "Path.hpp"
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "glm/glm.hpp"
#include "cAssimpModel.h"

struct TurretLogic;
struct Weapon;
struct Turret {
  Turret(Weapon const* aWeapon);
  ~Turret();
  
  bool Load(pugi::xml_node const& aTurretNode);
  void LoadModel(char const*const filename);

protected:
  Weapon const* mWeapon;
  cAssimpModel mAssimpModel;
  float mBuildingTime;
  friend struct TurretLogic;
};

struct WeaponLogic;
struct EnemyLogic;
struct TurretLogic {
  TurretLogic(Turret const*const turret, WeaponLogic* aWeaponLogic);
  ~TurretLogic();

  virtual void Attack(EnemyLogic* const enemy, float const timeMS);
  virtual bool CanHit(glm::vec2 const& apos, float const timeMS);
  void init(float const time_ms);

  void Render() const;

  float getBuildingDuration() const { return mTurret->mBuildingTime; }

protected:
  Turret const* mTurret;
  WeaponLogic* mWeaponLogic;
  glm::vec2 mPosition{-1,-1};
};

