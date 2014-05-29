#pragma once
#include <string>
#include "pugixml.hpp"
#include "Weapon.hpp"
#include "cAssimpModel.h"

struct EnemyLogic;
struct Enemy {
  Enemy();
  ~Enemy();

  bool Load(pugi::xml_node aEnemyNode);
  void LoadModel(char const*const filename);

protected:
  friend struct EnemyPlayer;
  float mLife=-1;
  float mTilesPerMinute;
  std::string mWeaponUniqueID;
  cAssimpModel mAssimpModel;
  friend struct EnemyLogic;
};

struct EnemyLogic {
  /// @param aLevel not owning pointer
  EnemyLogic(Enemy const*const anEnemy, WeaponLogic&& awl);
  ~EnemyLogic();
  EnemyLogic& operator=(EnemyLogic const& ot);

  ///Initializes at specified time point
  void init(float const time_ms);
  ///Advances time
  //bool advanceTime(float const dt_ms);
  void ReceiveDamage(float const damage);

  bool hasDied() const { return mActualLife<=0; }

  void Render() const;
  glm::vec2 const& getPosition() const { return mPosition; }
protected:
  Enemy const* mEnemy;
  WeaponLogic mWeaponLogic;
  float mActualLife;
  glm::vec2 mPosition{-1,-1};
  float mLastMovedMS=-1;
};