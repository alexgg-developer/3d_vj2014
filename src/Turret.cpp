#include "Turret.hpp"
#include <iostream>
#include <algorithm>
#include "Enemy.hpp"

Turret::Turret(Weapon const* aWeapon, float const aBuildingTime, float const aMonetaryCost)
  : mWeapon(aWeapon), mBuildingTime(aBuildingTime), mMonetaryCost(aMonetaryCost) {}
Turret::~Turret() {}

void Turret::LoadModel(char const*const filename) {
  if(!mAssimpModel.LoadFromFile(filename)) {
    std::cout << "Model " << filename << " can't be loaded." << std::endl;
    assert(0);
  }
}



TurretLogic::TurretLogic(Turret const*const turret, Weapon const*const aWeapon)
  : mTurret(turret), mWeaponLogic(aWeapon) {}
TurretLogic::~TurretLogic() {}
void TurretLogic::Render() const {
	glPushMatrix();
	glTranslatef(-mPosition.x, 0.5f, -mPosition.y);
  mTurret->mAssimpModel.Render();
	glPopMatrix();
}

void TurretLogic::Attack(EnemyLogic* const enemy, float const timeMS) {
  assert(mWeaponLogic.CanHit(timeMS));
  mWeaponLogic.Attack(enemy,timeMS);
}
bool TurretLogic::CanHit(glm::vec2 const& apos, float const timeMS) {
  if(mWeaponLogic.CanHit(timeMS)) {
    float const manhattanDistance = std::abs(apos.x-mPosition.x) + std::abs(apos.y-mPosition.y);
    return manhattanDistance<=mTurret->mWeapon->get_radius_tiles();
  }
  return false;
}
void TurretLogic::init(float const time_ms) {
  mWeaponLogic.init(time_ms);
}