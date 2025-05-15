#include "voxel/chunk.hpp"
// #include "voxel/block.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;


class Rule {
	public:
		Rule(std::vector<vec3> relative_positions, std::vector<BlockType> relative_types, BlockType result, float probability);
		std::vector<vec3> relative_positions;
		std::vector<BlockType> relative_types;
		BlockType result;
		float probability;
		bool applies_to(const Chunk &C, const vec3& pos) const;
		void apply(Chunk &C, const vec3& pos) const;
};