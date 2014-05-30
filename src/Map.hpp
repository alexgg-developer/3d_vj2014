#pragma once
#include "Path.hpp"
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "glm/glm.hpp"

//Warning: x,y order changed!
struct Map {
  Map();
  ~Map();

  bool Load(pugi::xml_node const& mapNode);
  bool EnemyCanBeIn(int const x, int const y) const;

  protected:
  glm::vec2 mSize;
  enum TileType {BUILDABLE, PASSABLE, USELESS};
  std::vector<std::vector<TileType>> mTileTypeMatrix;
};

