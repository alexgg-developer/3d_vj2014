#include "Enemy.hpp"
#include <iostream>


Enemy::Enemy() {}
Enemy::~Enemy() {}

bool Enemy::Load(pugi::xml_node aEnemyNode) {
  mSpecies = aEnemyNode.attribute("species").value();
  mLife = aEnemyNode.attribute("life").as_float();
  mWeaponUniqueID = aEnemyNode.child("weapon_ref").attribute("name").value();
  mTilesPerMinute = aEnemyNode.attribute("tiles_per_minute").as_float();
  mMonetaryValue= aEnemyNode.attribute("monetary_value").as_float();
  std::string die_sound_name = aEnemyNode.attribute("die_sound").value();
  mDieSound.load(die_sound_name);
  std::string attack_sound_name = aEnemyNode.attribute("attack_sound").value();
  mAttackDefensorSound.load(attack_sound_name);
  std::string mModelPath = aEnemyNode.attribute("three_d_model").value();
  LoadModel(mModelPath.c_str());
  
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
  float const backup = mActualLife;
  mActualLife -= damage;
  if(hasDied() && backup>0)
    mEnemy->mDieSound.play();
}
void EnemyLogic::Attack(Defensor& df) {
  df.receiveDamage(mWeaponLogic.getDamage());
  mEnemy->mAttackDefensorSound.play();
}
void EnemyLogic::Render() const {
//TODO: Animation, rotation
	glPushMatrix();
	glTranslatef(-mPosition.x, 0.3f, -mPosition.y);
  //Apply ICE as scale reduction
  if(mTimeToStop>mTimeStopped) {
    float const last_ms = std::min(1000.0f,mTimeToStop-mTimeStopped);
    float const scale = std::max(0.2f,(1000.0f-last_ms)/1000.0f);
    glScalef(scale,scale,scale);
  }
  mEnemy->mAssimpModel.Render();
	glPopMatrix();
}