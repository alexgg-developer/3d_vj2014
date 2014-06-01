#include "Path.hpp"
#include "Enemy.hpp"
#include "Map.hpp"
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

  if(code!="attack-base") {
    pugi::xml_attribute attr = aOrderNode.attribute("quantity");
    assert(attr);
    mQuantity = attr.as_int();
  }
  return true;
}

#include "PathLogic.hpp"
#include "EnemyLogic.hpp"
PathLogic::PathLogic(Path const*const aPath, Defensor *const aDefensor, Map const*const aMap)
  : mPath(aPath), mDefensor(aDefensor), mMap(aMap), mControlledEnemies{} {
   //mControlledEnemies.reserve(100);
}
PathLogic::~PathLogic() {}
void PathLogic::assignEnemy(EnemyLogic* el) {
  mControlledEnemies.push_back(new EnemyMoving_(el));
  mControlledEnemies.back()->enemy->setPosition(mPath->mStartPosition);
  mControlledEnemies.back()->mNextPosition = mPath->mStartPosition;
  mControlledEnemies.back()->mPositionWhenStartedOrder = mPath->mStartPosition;
  ApplyNextOrderTo(*mControlledEnemies.back());
}
void PathLogic::advance_time(float const init_time_ms, float const dt_ms) {
  for(auto const& em : mControlledEnemies) {
    if(em->enemy->time_stopped() >= em->enemy->time_to_stop()) {
      glm::vec2 const deltaPos = glm::normalize(em->mNextPosition-em->mPositionWhenStartedOrder)*em->enemy->getVelocity_tiles_per_ms()*dt_ms;
      glm::vec2 const newPosition = em->enemy->getPosition() + deltaPos;
      //Check if enemy has advanced more than the specified quantity
      float const maxQuantity = static_cast<float>(mPath->mOrders[em->mActualOrder].mQuantity);
      glm::vec2 const delta = em->mPositionWhenStartedOrder - em->enemy->getPosition();
      float const actual_separation_squared = delta.x*delta.x + delta.y*delta.y;
      em->enemy->setPosition(newPosition);
      if( actual_separation_squared >= maxQuantity*maxQuantity) {
        //Next order
        ApplyNextOrderTo(*em);
      }
    }
  }
  //Delete finished enemies. TODO: Move them to a non-attacking exit path
  for(auto it = mControlledEnemies.begin(); it!=mControlledEnemies.end();) {
    if((*it)->enemy->mPathFinished || (*it)->enemy->hasDied()) {
      std::cout << "an enemy died" << std::endl;
      bool const died = (*it)->enemy->hasDied();
      (*it)->enemy->ReceiveDamage((*it)->enemy->life()+1.0f);
      delete *it;
      it = mControlledEnemies.erase(it);
    }
    else ++it;
  }
}
void PathLogic::ApplyNextOrderTo(EnemyMoving_& em) {
  if(em.mActualOrder==mPath->mOrders.size()-1) {
    em.enemy->mPathFinished=true;//finished this path
  } else {
    em.mActualOrder++;
    em.enemy->setPosition(em.mNextPosition);
    em.mPositionWhenStartedOrder = em.mNextPosition;//em.enemy->getPosition();
    Path::Order const& ord = mPath->mOrders[em.mActualOrder];
    if(ord.mOrderType==Path::Order::OrderType::DOWN      ) em.mNextPosition += glm::vec2(0,-ord.mQuantity);
    else if(ord.mOrderType==Path::Order::OrderType::UP   ) em.mNextPosition += glm::vec2(0,ord.mQuantity);
    else if(ord.mOrderType==Path::Order::OrderType::LEFT ) em.mNextPosition += glm::vec2(-ord.mQuantity,0);
    else if(ord.mOrderType==Path::Order::OrderType::RIGHT) em.mNextPosition += glm::vec2(ord.mQuantity,0);
    else if(ord.mOrderType==Path::Order::OrderType::ATTACK_BASE) {
      assert(!em.enemy->mPathFinished);
      em.enemy->Attack(*mDefensor);  //Attacks prota
      em.enemy->mPathFinished=true;//attack only once
    }
    else { 
      std::cout << "Order type not recognized " << ord.mOrderType << std::endl;
      assert(0);
    }
  }
}

//EnemyMoving::EnemyMoving(EnemyLogic* el)