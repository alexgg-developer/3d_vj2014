#pragma once
#include <string>
#include "Map.hpp"
#include "Avalancha.hpp"

struct Level {
  Level();
  ~Level();

  bool Load(std::string aFilename);

protected:
  Map mMap;
  std::vector<Path> mAssociatedPaths;
  std::vector<Avalancha*> mAvalanchas;
  friend struct LevelLogic;
};
