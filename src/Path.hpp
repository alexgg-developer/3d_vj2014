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

struct Defensor;
struct EnemyLogic;
struct Map;
struct PathLogic {
  PathLogic(Path const*const aPath, Defensor *const aDefensor, Map const*const aMap);
  ~PathLogic();

  void assignEnemy(EnemyLogic* el);
  void advance_time(float const init_time_ms, float const dt_ms);

protected:
  Path const* mPath;
  Defensor * mDefensor;
  Map const* mMap; 
  struct EnemyMoving {
    EnemyMoving(EnemyLogic* el);
    EnemyLogic* enemy=nullptr;
    int mActualOrder=-1;
    glm::vec2 mNextPosition;
    glm::vec2 mPositionWhenStartedOrder;
    //TODO: Orientation
  };
  std::vector<EnemyMoving> mControlledEnemies;
  void ApplyNextOrderTo(EnemyMoving& em);
};