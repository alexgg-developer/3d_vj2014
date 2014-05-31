#pragma once
#include <string>
#include <vector>
#include <map>
#include "pugixml.hpp"
#include <glm/glm.hpp>

struct LevelLogic;

/// Represents a path for enemies. It's an starting point and a set of rules that will guide direction of the enemies.
/// It's name is it's unique ID.
struct Avalancha {
  Avalancha();
  ~Avalancha();
  
  virtual float temporal_length() const =0;
  void ChangeOrder(int const aOrder) { mOrder = aOrder; }
  virtual std::map<std::string, unsigned int> HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const =0;
protected:
  ///this is outside data. It's copied here for debugging purposes.
  int mOrder;
};

Avalancha* BuildAvalancha(pugi::xml_node const& someAvalanchaNode);

struct SimpleAvalancha : public Avalancha {
  SimpleAvalancha();
  ~SimpleAvalancha();

  bool Load(pugi::xml_node const& aSimpleAvalanchaNode);
  virtual std::map<std::string, unsigned int> HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const override;
  
  virtual float temporal_length() const override { return mStartMiliSeconds + mTemporalLengthSeconds; }
protected:
  std::string mEnemyUniqueID;
  float mEnemiesPerMinute;
  float mTemporalLengthSeconds;
  float mStartMiliSeconds;
};

struct EmptyAvalancha : public Avalancha{
  EmptyAvalancha();
  ~EmptyAvalancha();

  bool Load(pugi::xml_node const& anEmptyAvalanchaNode);
  virtual std::map<std::string, unsigned int> HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const override { return std::map<std::string, unsigned int>(); }
  
  virtual float temporal_length() const override { return 0; }
protected:
  float mTemporalLengthSeconds;
};
#include <algorithm>
/// Avalancha formed by other avalanchas
struct CompoundAvalancha : public Avalancha{
  CompoundAvalancha();
  ~CompoundAvalancha();

  bool Load(pugi::xml_node const& aCompoundAvalanchaNode);
  virtual std::map<std::string, unsigned int> HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const override {
    std::map<std::string, unsigned int> ret;
    for(auto av: mSons) {
      auto ret2 = av->HowMuchToSpawn(t_avalancha_started_ms, init_t_ms, dt_ms, aLevel);
      for(auto const& p : ret2) {
        ret[p.first] += p.second;
      }
    }
    return ret;
  }
  virtual float temporal_length() const override { 
    float ret=0;
    for(auto av: mSons) {
      ret = std::max(ret,av->temporal_length());
    }
    return ret;
  }

protected:
  std::vector<Avalancha*> mSons;
};

struct Enemy;
struct Weapon;
struct LevelLogic;
struct AvalanchaLogic {
  AvalanchaLogic(Avalancha const*const av) : mAvalancha(av) {}

  void init(float time_ms) { mInitTime=time_ms; }
  /// Spawns new enemies at aLevel if needed
  void advance_time(float const init_t_ms, float const dt_ms, LevelLogic *const aLevel, std::vector<Enemy> const& availableEnemies, std::vector<Weapon> const& availableWeapons);
  bool has_ended(float const time_ms) const;
protected:
  Avalancha const* mAvalancha;
  float mInitTime=0;
};