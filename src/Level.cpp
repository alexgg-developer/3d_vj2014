#include "Level.hpp"
#include "pugixml.hpp"
#include <iostream>

Level::Level() { }
Level::~Level() { }

bool Level::Load(std::string aFilename) {
  pugi::xml_document doc;
  if (!doc.load_file(aFilename.c_str())) {
    std::cout << "Could not open file " << aFilename << " on Level::Load" << std::endl;
    return false;
  }
}
