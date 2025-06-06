#include "int3.hpp"

std::size_t Int3Hasher::operator()(const Int3 &k) const
{
  std::size_t h1 = std::hash<int>()(k.x);
  std::size_t h2 = std::hash<int>()(k.y);
  std::size_t h3 = std::hash<int>()(k.z);
  // Combine hashes (this is a basic technique, better ones exist)
  return h1 ^ (h2 << 1) ^ (h3 << 2);
}

Int3::Int3() : x(0), y(0), z(0)
{
}

Int3::Int3(int x, int y, int z) : x(x), y(y), z(z) {}

int correct_round(float x)
{
  int x_abs = std::abs(x);
  if (x >= 0)
  {
    return x_abs;
  }
  else
  {
    return -x_abs - 1;
  }
}

Int3::Int3(vec3 position)
{
  x = correct_round(position.x);
  y = correct_round(position.y);
  z = correct_round(position.z);
}

Int3 Int3::Neighboor(Directions dir) const
{
  switch (dir)
  {
  case Up:
    return Int3(x, y, z + 1);
  case Down:
    return Int3(x, y, z - 1);
  case Front:
    return Int3(x + 1, y, z);
  case Back:
    return Int3(x - 1, y, z);
  case Left:
    return Int3(x, y + 1, z);
  case Right:
    return Int3(x, y - 1, z);
  default:
    return Int3(x, y, z);
  }
}

vec3 Int3::ToVec() const
{
  return vec3(x, y, z);
}

float Int3::sqared_dist(const Int3 &other) const
{
  return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y) + (other.z - z) * (other.z - z);
}

int Int3::Manhattan(const Int3 &other) const
{
  return std::abs(other.x - x) + std::abs(other.y - y) + std::abs(other.z - z);
}

bool Int3::operator==(const Int3 &other) const
{
  return x == other.x && y == other.y && z == other.z;
}

Int3 Int3::operator+(const Int3 &other) const
{
  return Int3(x + other.x, y + other.y, z + other.z);
}
