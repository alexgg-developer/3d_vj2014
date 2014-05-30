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
  Turret(Weapon const* aWeapon, float const aBuildingTime=0, float const aMonetaryCost=10);
  ~Turret();
  
  //bool Load(pugi::xml_node const& aTurretNode);
  void LoadModel(char const*const filename);
  float MonetaryCost() const { return mMonetaryCost; }

protected:
  Weapon const* mWeapon;
  cAssimpModel mAssimpModel;
  float mBuildingTime;
  float mMonetaryCost;
  friend struct TurretLogic;
};

#include "Weapon.hpp"
struct EnemyLogic;
struct TurretLogic {
  TurretLogic(Turret const*const turret, Weapon const*const aWeapon);
  ~TurretLogic();

  virtual void Attack(EnemyLogic* const enemy, float const timeMS);
  virtual bool CanHit(glm::vec2 const& apos, float const timeMS);
  void init(float const time_ms);

  void Render() const;

  float getBuildingDuration() const { return mTurret->mBuildingTime; }
  glm::vec2 const& getPosition() const { return mPosition;}
  void setPosition(glm::vec2 const& p) { mPosition=p;}
protected:
  Turret const* mTurret;
  WeaponLogic mWeaponLogic;
  glm::vec2 mPosition{-1,-1};
};

