#pragma once
#include "Path.hpp"
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "glm/glm.hpp"

//Warning: x,y order changed!
struct Map {
  enum TileType {BUILDABLE, PASSABLE, USELESS};
  Map();
  ~Map();

  bool Load(pugi::xml_node const& mapNode);
  bool EnemyCanBeIn(int const x, int const y) const;
  
  std::size_t sizeX() const;
  std::size_t sizeY() const;

  TileType const& operator()(std::size_t x, std::size_t y) const;

protected:
  glm::vec2 mSize;
  std::vector<std::vector<TileType>> mTileTypeMatrix;
};

#include "cAssimpModel.h"
struct MapLogic {
  MapLogic(Map const*const aMap);
  bool init_and_load();
  void render() const;
  void CompileDisplayList();
  Map::TileType const& operator()(std::size_t x, std::size_t y) const { return (*mMap)(x,y);}
protected:
  Map const* mMap;
  cAssimpModel mModelBuildable, mModelPassable, mModelUseless;
  int displayList;
};

