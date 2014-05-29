#include "Path.hpp"
#include <iostream>

Path::Path() { }
Path::~Path() { }

bool Path::Load(pugi::xml_node const& aPathNode) {
  mName = aPathNode.attribute("name").value();
  mStartPosition.x = static_cast<float>(aPathNode.child("start_point").attribute("x").as_float());
  mStartPosition.y = static_cast<float>(aPathNode.child("start_point").attribute("y").as_int());
  
  
  for(pugi::xml_node const& aOrderNode : aPathNode.children("order")) {
    Order o;
    if(!o.Load(aOrderNode)) {
      std::cout << "Could not load order on path" << std::endl;
      assert(0);
      return false;
    }
    mOrders.push_back(o);
  }
  return true;
}


Path::Order::Order() {}
Path::Order::~Order() {}
bool Path::Order::Load(pugi::xml_node const& aOrderNode) {
  std::string code = aOrderNode.attribute("code").value();
  if(code=="up") mOrderType=OrderType::UP;
  else if(code=="down") mOrderType=OrderType::DOWN;
  else if(code=="left") mOrderType=OrderType::LEFT;
  else if(code=="right") mOrderType=OrderType::RIGHT;
  else if(code=="attack-base") mOrderType=OrderType::ATTACK_BASE;
  else {
    std::cout << "unknown OrderType code: " << code << std::endl;
    assert(0);
    return false;
  }

  pugi::xml_attribute attr = aOrderNode.attribute("quantity");
  if(attr) {
    mQuantity = attr.as_int();
    mIndefinitely=false;
  } else {
    mQuantity=-1;
    mIndefinitely=true;
  }
  return true;
}