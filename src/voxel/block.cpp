#include "block.hpp"

using namespace cgp;

vec3 Block::getColor() {
  switch (block_type) {
  case BlockType_Default:
    return vec3(1, 1, 1);

  case BlockType_Head:
    return vec3(1, 0, 0);

  case BlockType_Empty:
  default:
    return vec3(0, 0, 0);
  }
}

Block::Block() { block_type = BlockType_Empty; }
