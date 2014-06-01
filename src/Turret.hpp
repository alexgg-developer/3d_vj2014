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
  Turret(Weapon const* aWeapon, float const aBuildingTime, float const aMonetaryCost);
  ~Turret();
  
  //bool Load(pugi::xml_node const& aTurretNode);
  void LoadModel(char const*const filename);
  float MonetaryCost() const { return mMonetaryCost; }

protected:
  Weapon const* mWeapon;
  cAssimpModel mAssimpModel;
  float mBuildingTimeMS=3000.0f;
  float mMonetaryCost;
  friend struct TurretLogic;
};

#include "WeaponLogic.hpp"
struct EnemyLogic;
struct TurretLogic {
  TurretLogic(Turret const*const turret, Weapon const*const aWeapon);
  ~TurretLogic();

  virtual void Attack(EnemyLogic* const enemy, float const timeMS);
  virtual bool CanHit(glm::vec2 const& apos, float const timeMS);
  void init(float const time_ms);

  void Render() const;
  void setScale(float const sc);
  void setHeight(float const sc);

  float getBuildingDuration() const { return mTurret->mBuildingTimeMS; }
  glm::vec2 const& getPosition() const { return mPosition;}
  void setPosition(glm::vec2 const& p) { mPosition=p;}
protected:
  Turret const* mTurret;
  WeaponLogic mWeaponLogic;
  glm::vec2 mPosition{-1,-1};
  float mScale=1.0f;
  float mHeight=0.5f;
};

