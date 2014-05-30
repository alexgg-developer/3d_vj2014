#include "Map.hpp"
#include <iostream>
#include <algorithm>

Map::Map() { }
Map::~Map() { }

bool Map::Load(pugi::xml_node const& mapNode) {
  //Get size
  mSize.x = static_cast<float>(mapNode.attribute("map_sizeX").as_int());
  mSize.y = static_cast<float>(mapNode.attribute("map_sizeY").as_int());

  //Load map. It's a string formed by UBP
  std::string mapRaw = mapNode.attribute("map_tile_types").value();

  //Clean map from extrange characters
  std::string mapClean;
  std::copy_if(mapRaw.cbegin(), mapRaw.cend(), std::back_inserter(mapClean), [](char const c) {
    return c=='U' || c=='B'|| c=='P'; });

  //Check size
  if(mapClean.size() != mSize.x * mSize.y) {
    std::cout << "map is incomplete. It has " << mapClean.size() << " tiles and it should have " << mSize.x*mSize.y << std::endl;
    assert(0);
    return false;
  }

  //Resize tiles and read them
  mTileTypeMatrix.resize(static_cast<unsigned int>(mSize.x));
  for(auto& v: mTileTypeMatrix)
    v.resize(static_cast<unsigned int>(mSize.y));

  for (int y = 0; y < mSize.y; ++y) {
    for (int x = 0; x < mSize.x; ++x) {
      char c = mapClean[x + y*static_cast<unsigned int>(mSize.x)];
      if(c=='U') mTileTypeMatrix[x][y] = TileType::USELESS;
      else if(c=='P') mTileTypeMatrix[x][y] = TileType::PASSABLE;
      else if(c=='B') mTileTypeMatrix[x][y] = TileType::BUILDABLE;
      else { 
        std::cout << "parsing map: tile type not recognized: " << c << " on position " << x << "," << y << std::endl;
        assert(0);
        return false;
      }
    }
  }

  return true;
}
#include <algorithm>
bool Map::EnemyCanBeIn(int const x, int const y) const {
  int const x_index = std::max<int>(0, std::min<int>(x, mTileTypeMatrix.size()-1));
  int const y_index = std::max<int>(0,std::min<int>(y, mTileTypeMatrix[0].size()-1));
  return operator()(x_index, y_index) == TileType::PASSABLE;
}
std::size_t Map::sizeX() const {
  return mTileTypeMatrix[0].size();}
std::size_t Map::sizeY() const {
  return mTileTypeMatrix.size();}

Map::TileType const& Map::operator()(std::size_t x, std::size_t y) const {
  return this->mTileTypeMatrix[y][x];
}

MapLogic::MapLogic(Map const*const aMap) : mMap(aMap) {}
bool MapLogic::init_and_load() {
  bool ret = true;
  ret &= mModelBuildable.LoadFromFile("./objs/mountain/mount.blend12.obj");
  ret &= mModelPassable.LoadFromFile("./objs/pallet.obj");
  ret &= mModelUseless.LoadFromFile("./objs/roundabout-dec.obj");
  assert(ret);
  CompileDisplayList();
  return ret;
}
void MapLogic::CompileDisplayList() {
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
  glMatrixMode(GL_MODELVIEW);
  for(std::size_t x=0; x<mMap->sizeX(); ++x) {
    for(std::size_t y=0; y<mMap->sizeY(); ++y) {
      Map::TileType const& tt = (*mMap)(x,y);
      glPushMatrix();
      glTranslatef(-static_cast<float>(x), -0.9f, -static_cast<float>(y));
      if(tt==Map::TileType::BUILDABLE) {
        mModelBuildable.RenderRaw();
      } else if(tt==Map::TileType::PASSABLE) {
        mModelPassable.RenderRaw();
      } else if(tt==Map::TileType::USELESS) {
        mModelUseless.RenderRaw();
      } else {
        std::cout << "Non recognized tile type" << std::endl;
        assert(0);
      }
      glPopMatrix();
    }
  }
	glEndList();
}
void MapLogic::render() const {
	glCallList(displayList);
}