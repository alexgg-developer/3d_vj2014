#include "Level.hpp"
#include "pugixml.hpp"
#include <iostream>

Level::Level() { }
Level::~Level() { }

bool Level::Load(std::string aFilename) {
  //Open xml
  pugi::xml_document doc;
  if (!doc.load_file(aFilename.c_str())) {
    std::cout << "Could not open file " << aFilename << " on Level::Load" << std::endl;
    return false;
  }
  
  //Load map
  if(!mMap.Load(doc.child("level").child("map"))) {
    std::cout << "Level loading error: could not load <map>, son of <level> on " << aFilename << std::endl;
    assert(0);
    return false;
  }

  //Load paths
  for(pugi::xml_node const& pathNode : doc.child("level").child("paths").children("path")) {
    Path p;
    bool const ret_val = p.Load(pathNode);
    if(!ret_val) {
      std::cout << "Could not load a <path> node while loading a <level><paths>" << std::endl;
      assert(0);
      return false;
    }
    mAssociatedPaths.push_back(p);
  }

  //Load avalanchas
  for(pugi::xml_node const& avalanchaNode : doc.child("level").child("avalanchas").children()) {
    Avalancha *const p = BuildAvalancha(avalanchaNode);
    if(p==nullptr) {
      std::cout << "Could not load an <avalancha> node while loading a <level><avalanchas>" << std::endl;
      assert(0);
      return false;
    }
    mAvalanchas.push_back(p);
  }
  return true;
}





LevelLogic::LevelLogic(Level const*const aLevel, Defensor *const aDefensor)
    : mLevel(aLevel), mDefensor(aDefensor), mMap(&aLevel->mMap) {
  mEnemies.reserve(1000);//TODO: If there are more than a thousand enemies it's location will be changed
  mAliveTurrets.reserve(1000);
  mBuildingTurrets.reserve(1000);
  mEnemies.reserve(1000);
  mPaths.reserve(1000);
  mAvalanchas.reserve(1000);
}
LevelLogic::~LevelLogic() {}

///Initializes at specified time point
void LevelLogic::init(float const time_ms) {
  mBuyTurret.load("./music/several_coins_placed_lightly_down_on_table.wav");
  //No-insert alive turrets by default
  for(auto& a:mAliveTurrets) a.init(time_ms);
  for(auto& a:mBuildingTurrets) std::get<0>(a).init(time_ms);
  for(auto& a:mEnemies) a.init(time_ms);
  //Insert paths
  for(Path const& p : mLevel->mAssociatedPaths) {
    PathLogic pl(&p, mDefensor, &mLevel->mMap);
    //pl.init(time_ms);
    mPaths.push_back(pl);
  }
  //Insert avalanchas
  for(Avalancha const*const av : mLevel->mAvalanchas) {
    AvalanchaLogic al(av);
    al.init(time_ms);
    mAvalanchas.push_back(al);
  }
  this->mMap.init_and_load();
}
///Advances time
bool LevelLogic::advanceTime(Defensor& theDefensor, float const init_time_ms, float const dt_ms, std::vector<Enemy> const& availableEnemies, std::vector<Weapon> const& availableWeapons) {
  float const end_time_ms = init_time_ms + dt_ms;

  /// Build turrets
  for(std::vector<std::tuple<TurretLogic, float >>::iterator it = mBuildingTurrets.begin(); it!=mBuildingTurrets.end();) {
    std::tuple<TurretLogic, float > &tupl = *it;
    TurretLogic const& turret = std::get<0>(tupl);
    float const init_build_time_ms = std::get<1>(tupl);
    if(turret.getBuildingDuration() + init_build_time_ms <= end_time_ms) {
      mAliveTurrets.push_back(turret);
      it = mBuildingTurrets.erase(it);
    } else ++it;
  }

  /// Turrets attacks enemies
  for(EnemyLogic& enemy : mEnemies) {
    for (TurretLogic& turret : mAliveTurrets) {
      if(!enemy.hasDied() && turret.CanHit(enemy.getPosition(), end_time_ms)) {
        turret.Attack(&enemy, end_time_ms);
      }
      //if(!enemy.hasDied() && ) //Enemies can't attack towers, although it would be easy and fun
    }
  }

  /// Died enemies dissapear. They can be moved out to another collection and fade out or something like that.
  //delete died enemies from vector, it's a quadratic search!
  //they do not die so often and not so much at the same time
  for(std::vector<EnemyLogic>::iterator it = mEnemies.begin(); it!=mEnemies.end();) {
    if (it->hasDied()) {
      theDefensor.add_money(it->getEnemyMonetaryValue());
      it = mEnemies.erase(it);
    }
    else ++it;
  }
  
  //Avalanchas advance
  for(AvalanchaLogic & al : mAvalanchas) {
    al.advance_time(init_time_ms, dt_ms, this, availableEnemies, availableWeapons);
  }
  
  //Paths advance
  for(PathLogic & al : mPaths) {
    al.advance_time(init_time_ms, dt_ms);
  }
  return true;
}

void LevelLogic::Render() const {
  for(TurretLogic const& tu : mAliveTurrets) {
    tu.Render();
  }
/*  for(auto& tu : mBuildingTurrets) {
    //tu->Render();
    //TODO: Draw with alfa
  }*/
  for(EnemyLogic const& el : mEnemies) {
    el.Render();
  }

  this->mMap.render();
}

void LevelLogic::spawnsEnemy(EnemyLogic const& el) {
  mEnemies.push_back(el);
  assert(!mPaths.empty() && "There should be some path there");
  mPaths[rand()%mPaths.size()].assignEnemy(&mEnemies.back());
}
void LevelLogic::spawnsTurret(TurretLogic&& el) {
  this->mAliveTurrets.push_back(el);
  mBuyTurret.play();
}