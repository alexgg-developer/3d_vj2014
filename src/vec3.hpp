#pragma once

struct vec3 {
  float x, y, z;

  vec3() {};
  vec3(vec3 const & v) : x(v.x), y(v.y), z(v.z) {}
  vec3 (float const xPos, float const  yPos, float const zPos = 0) : x(xPos), y(yPos), z(zPos) {}
  vec3 operator+(vec3 const& ot) const {
    return vec3(x+ot.x, y+ot.y, z+ot.z);
  }
  vec3 operator*(float const f) const {
    return vec3(x*f, y*f, z*f);
  }
  vec3 & operator+=(const vec3 &v) {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
  }
  vec3 operator-() const
  {
    return vec3(-x,-y,-z);
  }
  bool operator==(const vec3 &v) const
  {
      return (x == v.x && y == v.y && z == v.z);
  }
};

//TODO: r, g, b poiting to x,y,z
