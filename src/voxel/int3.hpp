#pragma once
#include "cgp/cgp.hpp"

using namespace cgp;

enum Directions
{
  Up,    //  0,  0,  1
  Down,  //  0,  0, -1
  Front, //  1,  0,  0
  Back,  // -1,  0,  0
  Left,  //  0,  1,  0
  Right  //  0, -1,  0
};

struct Int3
{
  int x;
  int y;
  int z;

  Int3();
  Int3(int x, int y, int z);
  Int3(vec3 position);

  Int3 Neighboor(Directions dir) const;
  vec3 ToVec() const;

  float sqared_dist(const Int3 &other) const;
  int Manhattan(const Int3 &other) const;

  bool operator==(const Int3 &other) const;
  Int3 operator+(const Int3 &other) const;
};

struct Int3Hasher
{
  std::size_t operator()(const Int3 &k) const;
};