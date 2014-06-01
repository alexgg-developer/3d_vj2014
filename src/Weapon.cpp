#include "Weapon.hpp"
#include <iostream>
#include "Enemy.hpp"

Weapon::Weapon() {
}
Weapon::~Weapon() {}

bool Weapon::Load(pugi::xml_node aWeaponNode) {
  mName = aWeaponNode.attribute("name").value();
  mBulletsPerMinute = aWeaponNode.attribute("fire_rate_per_minute").as_float();
  mDamage = aWeaponNode.attribute("damage_per_bullet").as_float();
  mRadiusTiles = aWeaponNode.attribute("range_tiles").as_float();
  mSpecialEffect = SpecialEffect::NONE;
  std::string audio_name = aWeaponNode.attribute("audio_file").value();
  mShootWeapon.load(audio_name);

  pugi::xml_attribute special_effect = aWeaponNode.attribute("special_effect");
  if(special_effect) {
    std::string const code = special_effect.value();
    if(code=="ice")
      mSpecialEffect = SpecialEffect::ICE;
    else if(code=="burn")
      mSpecialEffect = SpecialEffect::BURN;
    else {
      std::cout << "Special effect code type " << code << " not recognized" << std::endl;
      assert(0);
      return false;
    }
    mSpecialEffectDuration = aWeaponNode.attribute("special_effect_duration_miliseconds").as_float();
  }
  
  return true;
}
float Weapon::miliseconds_per_bullet() const {
  float const MinutesPerMilisecond = 1.0f/(60.0f*1000.0f);
  return 1.0f/(mBulletsPerMinute*MinutesPerMilisecond);
}

#include "WeaponLogic.hpp"
#include "EnemyLogic.hpp"
WeaponLogic::WeaponLogic(Weapon const*const aWeapon) : mWeapon(aWeapon) {}
WeaponLogic::~WeaponLogic() {}
float WeaponLogic::getDamage() const {
  return mWeapon->mDamage; }

bool WeaponLogic::CanHit(float const time_ms) {
  return mLastShotMS+mWeapon->miliseconds_per_bullet()<=time_ms;
}
void WeaponLogic::Attack(EnemyLogic*const en, float const timeMS) {
  assert(CanHit(timeMS));
  en->ReceiveDamage(mWeapon->mDamage);
  mLastShotMS = timeMS;
  mWeapon->mShootWeapon.play();

  if(mWeapon->mSpecialEffect==Weapon::SpecialEffect::ICE) {
    //can't move enemy
    en->add_time_to_stop(this->mWeapon->mSpecialEffectDuration);
  }
}
void WeaponLogic::init(float const time_ms) {
  mLastShotMS = time_ms;
}