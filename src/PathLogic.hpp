#pragma once
#include "EnemyLogic.hpp"
#include <memory>
#include <list>

struct EnemyMoving_ {
  EnemyMoving_(EnemyLogic*const el) : enemy(el) {}
  ~EnemyMoving_() { }

  EnemyLogic* enemy=nullptr;
  int mActualOrder=-1;
  glm::vec2 mNextPosition;
  glm::vec2 mPositionWhenStartedOrder;
  //TODO: Orientation
};

/*  EnemyMoving em(nullptr);
  std::shared_ptr<EnemyMoving> sharedptr;
  std::unique_ptr<EnemyMoving> uniqueptr;
  std::list<std::unique_ptr<EnemyMoving>> mControlledEnemies;*/

struct Path;
struct Map;
struct PathLogic {
  std::vector<EnemyMoving_*> mControlledEnemies;
  PathLogic(Path const*const aPath, Defensor *const aDefensor, Map const*const aMap);
  ~PathLogic();

  void assignEnemy(EnemyLogic* el);
  void advance_time(float const init_time_ms, float const dt_ms);

protected:
  Path const* mPath;
  Defensor * mDefensor;
  Map const* mMap; 
  void ApplyNextOrderTo(EnemyMoving_& em);
};
