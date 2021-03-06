#pragma once
#include <string>
#include "pugixml.hpp"
#include "glm/glm.hpp"
#include "cSound.hpp"

struct Weapon {
  Weapon();
  ~Weapon();

  bool Load(pugi::xml_node aFilename);

  float miliseconds_per_bullet() const;
  float get_radius_tiles() const { return mRadiusTiles; }
  std::string const& getName() const { return mName; }
protected:
  float mBulletsPerMinute=1000;
  float mDamage=1;
  float mRadiusTiles=1.5;
  float mSpecialEffectDuration=0;
  std::string mName;
  enum SpecialEffect { NONE, ICE, BURN } mSpecialEffect;
  friend struct WeaponLogic;
  Sound mShootWeapon;
};

template<typename BI>
void loadWeapons(char const*const filename, BI bi) {
  pugi::xml_document doc;
  auto parse_res = doc.load_file(filename);
  if(!parse_res) {
    std::cout << "could not load file " << filename << " as an XML document due to " << parse_res.description() << std::endl;
    assert(0);
  }
  for(pugi::xml_node wn : doc.child("weapon_list").children("weapon")) {
    Weapon w;
    bool const ret = w.Load(wn);
    if(!ret) assert(0);
    bi = w;
    ++bi;
  }
}
