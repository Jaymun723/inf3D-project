#include "voxel/chunk.hpp"
// #include "voxel/block.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;


class Rule {
	public:
		Rule(std::vector<vec3> relative_positions, BlockType result);
		std::vector<vec3> relative_positions;
		BlockType result;
		bool applies_to(const vec3& pos) const;
		void apply(const vec3& pos) const;
};