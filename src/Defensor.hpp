#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "pugixml.hpp"
#include "cSound.hpp"
#include "cInput.hpp"
#include "LevelLogic.hpp"
#include "Turret.hpp"
#include "Weapon.hpp"

/// Represents a path for enemies. It's an starting point and a set of rules that will guide direction of the enemies.
/// It's name is it's unique ID.
struct Defensor {
  Defensor(float const maxLife, float const initialMoneyUnits) : mMaxLife(maxLife), mActualLife(maxLife), mMoney(initialMoneyUnits) {}
  ~Defensor();

  bool IsAlive() const { return mActualLife>0; }
  void init();
  void receiveDamage(float const dam);
  void receive_input(float const end_frame_t, Input& in, LevelLogic& ll, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix, std::vector<Turret> const& aTurrets, std::vector<Weapon> const& aWeapons);
  void add_money(float const m) { mMoney+=m;}
  /// changes the selected turret and weapon
  void set_selected_turret(std::size_t const turret) {
    mSelectedTurretIndex = turret; }
  float getLife() const {
   return mActualLife; }
  void restoreLife() { mActualLife = std::max(mMaxLife, mActualLife); }
  void restoreMoney(float const aMoney) { mMoney = std::max(aMoney, mMoney); }
  float getMoney() const { return mMoney;}
protected:
  float mMaxLife;
  float mActualLife;
  float mMoney;
  Sound mNotEnoughMoney;
  std::size_t mSelectedTurretIndex=0;
};
