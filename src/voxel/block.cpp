#include "block.hpp"

using namespace cgp;

vec3 Block::getColor() {
  switch (block_type) {
  case BlockType_Default:
    return vec3(1, 1, 1);

  case BlockType_Head:
    return vec3(1, 0, 0);

  case BlockType_Grass_Tmp:
	return vec3(0, 1, 0);

  case BlockType_Grass:
    return vec3(0.071, 0.561f, 0.02f);

  case BlockType_Wood_Tmp:
      return vec3(0.5f, 0.25f, 0);

  case BlockType_Wood:
	  return vec3(0.5f, 0.25f, 0);

  case BlockType_Leaf_Tmp:
	  return vec3(0, 1, 0.2f);

  case BlockType_Leaf:
      return vec3(0.361f, 0.671f, 0.329f);


  case BlockType_Rainbow:
      return vec3(rand_uniform(0.0f, 1.0f), rand_uniform(0.0f, 1.0f), rand_uniform(0.0f, 1.0f));

  case BlockType_Water:
	  return vec3(0.0f, 0.0f, 1.0f);

  case BlockType_Plank_Tmp:
      return vec3(1, 0, 0);

  case BlockType_Plank:
	  return vec3(0.851f, 0.996f, 1.0f);

  case BlockType_Flat_Roof:
	  return vec3(0, 0, 0);

  case BlockType_Pillar:
	  return vec3(0.9f, 0.9f, 0.9f);

  case BlockType_Road_Tmp:
	  return vec3(0.5f, 0.5f, 0.5f);

  case BlockType_Road:
	  return vec3(0.5f, 0.5f, 0.5f);

  case BlockType_White_Line_Tmp:
	  return vec3(1, 0, 1);

  case BlockType_White_Line:
	  return vec3(1, 1, 1);

  case BlockType_Sidewalk:
	  return vec3(0.8f, 0.8f, 0.8f);

  case BlockType_Black:
	  return vec3(0, 0, 0);

  case BlockType_Grey:
	  return vec3(0.5f, 0.5f, 0.5f);

  case BlockType_White:
	  return vec3(1, 1, 1);

  case BlockType_Empty:
  default:
    return vec3(0, 0, 0);
  }
}

Block::Block() { block_type = BlockType_Empty; }
