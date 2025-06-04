#include "block.hpp"

using namespace cgp;

vec3 Block::getColor() {
  switch (block_type) {
  case BlockType_Default:
    return vec3(1, 1, 1);

  case BlockType_Head:
    return vec3(1, 0, 0);

  case BlockType_Grass_Tmp:
	return vec3(0, 0.5f, 0.2f);

  case BlockType_Grass:
    return vec3(0, 1, 0);

  case BlockType_Wood_Tmp:
      return vec3(0.5f, 0.25f, 0);

  case BlockType_Wood:
	  return vec3(0.5f, 0.25f, 0);

  case BlockType_Leaf_Tmp:
	  return vec3(0, 1, 0.2f);

  case BlockType_Leaf:
      return vec3(0, 1, 0.2f);


  case BlockType_Rainbow:
      return vec3(rand_uniform(0.0f, 1.0f), rand_uniform(0.0f, 1.0f), rand_uniform(0.0f, 1.0f));

  case BlockType_Water:
	  return vec3(0.0f, 0.0f, 1.0f);

  case BlockType_Plank_Tmp:
      return vec3(1, 0, 0);

  case BlockType_Plank:
	  return vec3(0.8f, 0.52f, 0.25f);

  case BlockType_Empty:
  default:
    return vec3(0, 0, 0);
  }
}

Block::Block() { block_type = BlockType_Empty; }
