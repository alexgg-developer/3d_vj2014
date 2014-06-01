#pragma once
#include <string>
#include "pugixml.hpp"
#include "cAssimpModel.h"
#include "cSound.hpp"

struct Enemy {
  Enemy();
  ~Enemy();

  bool Load(pugi::xml_node aEnemyNode);
  void LoadModel(char const*const filename);
  std::string const& getName() const { return mSpecies; }
  std::string const& getWeaponName() const { return mWeaponUniqueID; }

protected:
  friend struct EnemyPlayer;
  float mLife=-1;
  float mTilesPerMinute;
  std::string mWeaponUniqueID;
  cAssimpModel mAssimpModel;
  std::string mSpecies;
  friend struct EnemyLogic;
  float mMonetaryValue=2;
  Sound mDieSound, mAttackDefensorSound;
};
template<typename BI>
void loadEnemies(char const*const filename, BI bi) {
  pugi::xml_document doc;
  auto parse_res = doc.load_file(filename);
  if(!parse_res) {
    std::cout << "could not load file " << filename << " as an XML document due to " << parse_res.description() << std::endl;
    assert(0);
    return;
  }
  for(pugi::xml_node wn : doc.child("enemies_list").children("enemy")) {
    Enemy w;
    bool const ret = w.Load(wn);
    if(!ret) assert(0);
    bi = w;
    ++bi;
  }
}
