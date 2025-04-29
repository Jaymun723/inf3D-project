#include "voxel/chunk.hpp"
// #include "voxel/block.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;


class Rule {
	public:
		Rule(std::vector<vec3> relative_positions, std::vector<BlockType> relative_types, BlockType result);
		std::vector<vec3> relative_positions;
		std::vector<BlockType> relative_types;
		BlockType result;
		bool applies_to(Chunk C, const vec3& pos) const;
		void apply(Chunk C, const vec3& pos) const;
};