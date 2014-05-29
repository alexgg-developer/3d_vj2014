#pragma once
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <glm/glm.hpp>

/// Represents a path for enemies. It's an starting point and a set of rules that will guide direction of the enemies.
/// It's name is it's unique ID.
struct Defensor {
  Defensor(float const maxLife, int const initialMoneyUnits) : mMaxLife(maxLife), mActualLife(maxLife), mMoney(initialMoneyUnits) {}
  ~Defensor();

  void receiveDamage(float const dam) { mActualLife -= dam; }
protected:
  float mMaxLife;
  float mActualLife;
  int mMoney;
};
