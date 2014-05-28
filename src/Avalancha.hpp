#pragma once
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <glm/glm.hpp>

/// Represents a path for enemies. It's an starting point and a set of rules that will guide direction of the enemies.
/// It's name is it's unique ID.
struct Avalancha {
  Avalancha();
  ~Avalancha();
  
  void ChangeOrder(int const aOrder) { mOrder = aOrder; }
protected:
  ///this is outside data. It's copied here for debugging purposes.
  int mOrder;
};

Avalancha* BuildAvalancha(pugi::xml_node const& someAvalanchaNode);

struct SimpleAvalancha : public Avalancha {
  SimpleAvalancha();
  ~SimpleAvalancha();

  bool Load(pugi::xml_node const& aSimpleAvalanchaNode);

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

protected:
  float mTemporalLengthSeconds;
};

/// Avalancha formed by other avalanchas
struct CompoundAvalancha : public Avalancha{
  CompoundAvalancha();
  ~CompoundAvalancha();

  bool Load(pugi::xml_node const& aCompoundAvalanchaNode);

protected:
  std::vector<Avalancha*> mSons;
};