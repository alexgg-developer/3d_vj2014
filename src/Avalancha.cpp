#include "Avalancha.hpp"
#include <iostream>

Avalancha::Avalancha() : mOrder(-1) {}
Avalancha::~Avalancha() {}

Avalancha* BuildAvalancha(pugi::xml_node const& someAvalanchaNode) {
  std::string const nodename = someAvalanchaNode.name();
  if(nodename=="simple_avalancha") {
    SimpleAvalancha* sa = new SimpleAvalancha();
    bool const ret = sa->Load(someAvalanchaNode);
    if(!ret) {
      std::cout << "Error loading a simple avalancha" << std::endl;
      assert(0);
      return false; }
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
    Avalancha*const a = BuildAvalancha(av);
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