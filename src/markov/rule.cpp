#include "rule.hpp"

BasicR::BasicR(std::vector<vec3> relative_positions, std::vector<BlockType> relative_types, BlockType result, float probability) : relative_positions(std::move(relative_positions)),
																																																																	 relative_types(std::move(relative_types)),
																																																																	 result(result),
																																																																	 probability(probability) {}

bool BasicR::applies_to(const Chunk &C, const vec3 &pos) const
{
	for (int i = 0; i < relative_positions.size(); ++i)
	{
		vec3 relative_pos = relative_positions[i];
		int x = pos.x + relative_pos.x;
		int y = pos.y + relative_pos.y;
		int z = pos.z + relative_pos.z;
		if (x < 0 || x >= C.BLOCK_CHUNK_SIZE.x || y < 0 || y >= C.BLOCK_CHUNK_SIZE.y || z < 0 || z >= C.BLOCK_CHUNK_SIZE.z)
		{
			return false;
		}
		if (C.m_pBlocks[x][y][z].block_type != relative_types[i])
		{
			return false;
		}
	}
	float x = ((float)rand() / (float)(RAND_MAX));
	return (x < probability);
}

void BasicR::apply(Chunk &C, const vec3 &pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = result;
}
