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
  virtual float prepared_en_time_ms() const =0;
  void ChangeOrder(int const aOrder) { mOrder = aOrder; }
  virtual std::map<std::string, unsigned int> HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const =0;
protected:
  ///this is outside data. It's copied here for debugging purposes.
  int mOrder;
};

Avalancha* BuildAvalancha(pugi::xml_node const& someAvalanchaNode, float const accum_time_ms);

struct SimpleAvalancha : public Avalancha {
  SimpleAvalancha();
  ~SimpleAvalancha();

  bool Load(pugi::xml_node const& aSimpleAvalanchaNode);
  virtual std::map<std::string, unsigned int> HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const override;
  
  virtual float temporal_length() const override { return mTemporalLengthSeconds*1000.0f; }
  virtual float prepared_en_time_ms() const override {return temporal_length()+ mStartMiliSeconds; }
  void add_to_start_ms(float const dt_ms) { mStartMiliSeconds += dt_ms; }
protected:
  std::string mEnemyUniqueID;
  float mEnemiesPerMinute=0;
  float mTemporalLengthSeconds=0;
  float mStartMiliSeconds=0;
};

struct EmptyAvalancha : public Avalancha{
  EmptyAvalancha();
  ~EmptyAvalancha();

  bool Load(pugi::xml_node const& anEmptyAvalanchaNode);
  virtual std::map<std::string, unsigned int> HowMuchToSpawn(float const t_avalancha_started_ms, float const init_t_ms, float const dt_ms, LevelLogic *const aLevel) const override { return std::map<std::string, unsigned int>(); }
  
  virtual float temporal_length() const override { return mTemporalLengthSeconds*1000.0f; }
  virtual float prepared_en_time_ms() const override {return temporal_length(); }
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
  void add_to_start_ms(float const dt_ms) { mStartMiliSeconds += dt_ms; }
  virtual float prepared_en_time_ms() const override {return temporal_length()+ mStartMiliSeconds; }

protected:
  std::vector<Avalancha*> mSons;
  float mStartMiliSeconds=0;
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