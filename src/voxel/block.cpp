#include "block.hpp"

Block::Block() { 
	active = false; 
	block_type = BlockType_Default;
}

void Block::setActive(bool active) {
	active = active;
}

void setBlockType(BlockType block_type) {
	block_type = block_type;
}



