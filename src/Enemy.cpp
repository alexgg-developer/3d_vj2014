#include "Enemy.hpp"
#include <iostream>

Enemy::Enemy() {}
Enemy::~Enemy() {}

bool Enemy::Load(pugi::xml_node aEnemyNode) {
  mSpecies = aEnemyNode.attribute("species").value();
  mLife = aEnemyNode.attribute("life").as_float();
  mWeaponUniqueID = aEnemyNode.child("weapon_ref").attribute("name").value();
  mTilesPerMinute = aEnemyNode.attribute("tiles_per_minute").as_float();
  return true;
}
void Enemy::LoadModel(char const*const filename) {
  if(!mAssimpModel.LoadFromFile(filename)) {
    std::cout << "Model " << filename << " can't be loaded." << std::endl;
    assert(0);
  }
}


EnemyLogic& EnemyLogic::operator=(EnemyLogic const& ot) {
  mEnemy = ot.mEnemy;
  mActualLife = ot.mActualLife;
  mWeaponLogic = ot.mWeaponLogic;
  mPosition = ot.mPosition;
  mLastMovedMS = ot.mLastMovedMS;
  return *this;
}
EnemyLogic::EnemyLogic(Enemy const*const anEnemy, WeaponLogic&& awl)
 : mEnemy(anEnemy), mWeaponLogic(awl), mActualLife(anEnemy->mLife) {
}
EnemyLogic::~EnemyLogic() {}
void EnemyLogic::init(float const time_ms) { mLastMovedMS = time_ms; }
///Advances time
/*bool EnemyLogic::advanceTime(float const dt_ms) {

}*/
void EnemyLogic::ReceiveDamage(float const damage) {
  mActualLife -= damage;
}
void EnemyLogic::Attack(Defensor& df) {
  df.receiveDamage(mWeaponLogic.getDamage());
}
void EnemyLogic::Render() const {
//TODO: Animation, rotation
	glPushMatrix();
	glTranslatef(-mPosition.x, 0.5f, -mPosition.y);
  mEnemy->mAssimpModel.Render();
	glPopMatrix();
}