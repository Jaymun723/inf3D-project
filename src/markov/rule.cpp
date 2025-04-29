#include "rule.hpp"


Rule::Rule(std::vector<vec3> relative_positions, std::vector<BlockType> relative_types, BlockType result) {
	relative_positions = relative_positions;
	relative_types = relative_types;
	result = result;
	return;
}


bool Rule::applies_to(const Chunk C, const vec3& pos) const {
	for (int i = 0; i < relative_positions.size(); ++i) {
		vec3 relative_pos = relative_positions[i];
		int x = pos.x + relative_pos.x;
		int y = pos.y + relative_pos.y;
		int z = pos.z + relative_pos.z;
		if (x < 0 || x >= Chunk::CHUNK_SIZE || y < 0 || y >= Chunk::CHUNK_SIZE || z < 0 || z >= Chunk::CHUNK_SIZE) {
			return false; 
		}
		if (C.m_pBlocks[x][y][z].block_type != relative_types[i]) {
			return false; 
		}
	}
	return true;
}


void Rule::apply(Chunk C, const vec3& pos) const {
	C.m_pBlocks[pos.x][pos.y][pos.z].block_type = result;
}