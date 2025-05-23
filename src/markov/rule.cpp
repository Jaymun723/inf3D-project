#include "rule.hpp"


Rule::Rule(std::vector<vec3> relative_positions, std::vector<BlockType> relative_types, BlockType result, float probability): 
	relative_positions(std::move(relative_positions)),
	relative_types(std::move(relative_types)),
	result(result),
	probability(probability){ }


bool Rule::applies_to(const Chunk &C, const vec3& pos) const {
	for (int i = 0; i < relative_positions.size(); ++i) {
		vec3 relative_pos = relative_positions[i];
		int x = pos.x + relative_pos.x;
		int y = pos.y + relative_pos.y;
		int z = pos.z + relative_pos.z;
		if (x < 0 || x >= C.CHUNK_SIZE || y < 0 || y >= C.CHUNK_SIZE || z < 0 || z >= C.CHUNK_SIZE) {
			return false; 
		}
		if (C.m_pBlocks[x][y][z].block_type != relative_types[i]) {
			return false; 
		}
	}
	float x = ((float)rand() / (float)(RAND_MAX));
	return (x < probability);
}


bool Rule::applies_to(const Chunk& C, const vec3& pos, const Chunk& Cpx, const Chunk& Cmx, const Chunk& Cpy, const Chunk& Cmy, const Chunk& Cpz, const Chunk& Cmz) const {

	for (int i = 0; i < relative_positions.size(); ++i) {
		vec3 relative_pos = relative_positions[i];
		int x = pos.x + relative_pos.x;
		int y = pos.y + relative_pos.y;
		int z = pos.z + relative_pos.z;
		if (x < 0 && Cmx.m_pBlocks[x + C.CHUNK_SIZE][y][z].block_type != relative_types[i]) {
			return false;
		}
		else if (x >= C.CHUNK_SIZE && Cpx.m_pBlocks[x - C.CHUNK_SIZE][y][z].block_type != relative_types[i]) {
			return false;
		}
		else if (y < 0 && Cmy.m_pBlocks[x][y + C.CHUNK_SIZE][z].block_type != relative_types[i]) {
			return false;
		}
		else if (y >= C.CHUNK_SIZE && Cpy.m_pBlocks[x][y - C.CHUNK_SIZE][z].block_type != relative_types[i]) {
			return false;
		}
		else if (z < 0 && Cmz.m_pBlocks[x][y][z + C.CHUNK_SIZE].block_type != relative_types[i]) {
			return false;
		}
		else if (z >= C.CHUNK_SIZE && Cpz.m_pBlocks[x][y][z - C.CHUNK_SIZE].block_type != relative_types[i]) {
			return false;
		}
		else if (C.m_pBlocks[x][y][z].block_type != relative_types[i]) {
			return false;
		}
	}
	float x = ((float)rand() / (float)(RAND_MAX));
	return (x < probability);

}

void Rule::apply(Chunk &C, const vec3& pos) const {
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = result;
}