#pragma once
#include <string>

struct Level {
  Level();
  ~Level();

  bool Load(std::string aFilename);
};

