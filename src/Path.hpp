#pragma once
#include <string>
#include <vector>
#include "pugixml.hpp"
#include <glm/glm.hpp>

/// Represents a path for enemies. It's an starting point and a set of rules that will guide direction of the enemies.
/// It's name is it's unique ID.
struct Path {
  Path();
  ~Path();

  bool Load(pugi::xml_node const& aPathNode);

protected:
  ///unique ID. Must be enforced from the outside, wild, real world by a game designer. This fantasy sounds hard to maintain.
  std::string mName;
  glm::vec2 mStartPosition;

  struct Order {
    Order();
    ~Order();
    bool Load(pugi::xml_node const& aOrderNode);
  protected:
    enum OrderType {UP, LEFT, RIGHT, DOWN, ATTACK_BASE} mOrderType;
    int mQuantity=-1;
    friend struct PathLogic;
  };
  std::vector<Order> mOrders;
  friend struct PathLogic;
};
