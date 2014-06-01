#include "Avalancha.hpp"
#include <iostream>
#include "Level.hpp"
#include "Enemy.hpp"
#include "Weapon.hpp"

Avalancha::Avalancha() : mOrder(-1) {}
Avalancha::~Avalancha() {}

Avalancha* BuildAvalancha(pugi::xml_node const& someAvalanchaNode, float const accum_time_ms) {
  std::string const nodename = someAvalanchaNode.name();
  if(nodename=="simple_avalancha") {
    SimpleAvalancha* sa = new SimpleAvalancha();
    bool const ret = sa->Load(someAvalanchaNode);
    if(!ret) {
      std::cout << "Error loading a simple avalancha" << std::endl;
      assert(0);
      return false; }
      sa->add_to_start_ms(accum_time_ms);
    return sa;
  }
  else if(nodename=="empty_avalancha") {
    EmptyAvalancha* sa = new EmptyAvalancha();
    bool const ret = sa->Load(someAvalanchaNode);
    if(!ret) {
      std::cout << "Error loading an empty avalancha" << std::endl;
      assert(0);
      return false; }
    return sa;
  }
  else if(nodename=="compound_avalancha") {
    CompoundAvalancha* sa = new CompoundAvalancha();
    sa->add_to_start_ms(accum_time_ms);
    bool const ret = sa->Load(someAvalanchaNode);
    if(!ret) {
      std::cout << "Error loading a compund avalancha" << std::endl;
      assert(0);
      return false; }
    return sa;
  } else {
    std::cout << "Type of avalancha " << nodename << " unidentified" << std::endl;
    assert(0);
    return nullptr;
  }
  return nullptr; //Dead code
}

SimpleAvalancha::SimpleAvalancha() : Avalancha() {}
SimpleAvalancha::~SimpleAvalancha() {}

std::map<std::string, unsigned int> SimpleAvalancha::HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const {
  float const ms_per_enemy = 1.0f / (mEnemiesPerMinute / (60.0f*1000.0f));
  float const t_avalancha_started_ms_real = t_avalancha_started_ms + mStartMiliSeconds;
  float const end_t_ms = init_t_ms+dt_ms;
  //Avalancha has not started
  if(t_avalancha_started_ms_real > end_t_ms) return std::map<std::string, unsigned int>();
  float const avalancha_end_ms_real = t_avalancha_started_ms_real + mTemporalLengthSeconds*1000.0f;
  std::map<std::string, unsigned int> ret;
  for(float enemySpawnTime = t_avalancha_started_ms_real; enemySpawnTime<avalancha_end_ms_real; enemySpawnTime+=ms_per_enemy) {
    if(enemySpawnTime>=init_t_ms && enemySpawnTime<end_t_ms) {
      ret[mEnemyUniqueID]++;//auto-initialized to 0
    } else if(enemySpawnTime>=end_t_ms) return ret;//early return
  }
  return ret;
}

bool SimpleAvalancha::Load(pugi::xml_node const& aSimpleAvalanchaNode) {
  pugi::xml_attribute orderAttr = aSimpleAvalanchaNode.attribute("order");
  if(orderAttr) mOrder = orderAttr.as_uint();

  mEnemyUniqueID = aSimpleAvalanchaNode.child("enemy_ref").attribute("species").value();
  mEnemiesPerMinute = aSimpleAvalanchaNode.child("frequency").attribute("per_minute").as_float();
  mTemporalLengthSeconds = aSimpleAvalanchaNode.child("temporal_length").attribute("seconds").as_float();
  mStartMiliSeconds = aSimpleAvalanchaNode.attribute("start_time_ms").as_float();
  return true;
}

EmptyAvalancha::EmptyAvalancha() : Avalancha() {}
EmptyAvalancha::~EmptyAvalancha() {}

bool EmptyAvalancha::Load(pugi::xml_node const& anEmptyAvalanchaNode) {
  pugi::xml_attribute orderAttr = anEmptyAvalanchaNode.attribute("order");
  if(orderAttr) mOrder = orderAttr.as_uint();

  mTemporalLengthSeconds = anEmptyAvalanchaNode.child("temporal_length").attribute("seconds").as_float(); 
  return true;
}


CompoundAvalancha::CompoundAvalancha() : Avalancha() {}
CompoundAvalancha::~CompoundAvalancha() {}

bool CompoundAvalancha::Load(pugi::xml_node const& aCompoundAvalanchaNode) {
  pugi::xml_attribute orderAttr = aCompoundAvalanchaNode.attribute("order");
  if(orderAttr) mOrder = orderAttr.as_uint();

  for(auto& av : aCompoundAvalanchaNode.children()) {
    Avalancha*const a = BuildAvalancha(av, mStartMiliSeconds);
    if(a==nullptr) {
      std::cout << "Could not load some avalancha node inside some compound avalancha" << std::endl;
      assert(0);
      return false;
    }
    a->ChangeOrder(mOrder);
    mSons.push_back(a);
  }
  return true;
}
#include <algorithm>
#include "Enemy.hpp"
#include "Weapon.hpp"
bool AvalanchaLogic::has_ended(float const time_ms) const {
  float const length = mAvalancha->temporal_length();
  float const avalancha_end = length + mInitTime;
  return avalancha_end<time_ms;
}
//std::vector<Weapon> const& availableWeapons, 
void AvalanchaLogic::advance_time(float const init_t_ms, float const dt_ms, LevelLogic *const aLevel, std::vector<Enemy> const& availableEnemies, std::vector<Weapon> const& availableWeapons) {
  Avalancha const*const actualAvalancha = mAvalancha;
  std::map<std::string, unsigned int> const& toSpawn = mAvalancha->HowMuchToSpawn(mInitTime, init_t_ms, dt_ms, aLevel);
  for(auto const& p : toSpawn) {
    //search enemy
    std::string const enemyName = p.first;
    auto itE = std::find_if(availableEnemies.begin(), availableEnemies.end(),
      [&enemyName](Enemy const& en){ return en.getName() == enemyName; });
    if(itE==availableEnemies.end()) {
      std::cout << "Couldn't find enemy " << enemyName << std::endl;
      assert(0);
    } else {
      //search weapon
      std::string const weaponName = itE->getWeaponName();
      auto itW = std::find_if(availableWeapons.begin(), availableWeapons.end(),
        [&weaponName](Weapon const& en){ return en.getName() == weaponName; });
      if(itW==availableWeapons.end()) {
        std::cout << "Couldn't find weapon " << weaponName << std::endl;
        assert(0);
      } else {
        Weapon const& w = *itW;
        Enemy const& e = *itE;
        WeaponLogic wl(&w);
        EnemyLogic el(&e, std::move(wl));
        if(p.second>100) {
          std::cout << "Something went wrong, too much enemies are spawned on a single frame" << std::endl;
          assert(0);
        }
        for(std::size_t i=0; i<p.second; ++i)
          aLevel->spawnsEnemy(el);
      }
    }
  }
}