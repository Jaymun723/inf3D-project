// #include "voxel/chunk.hpp"
// #include "voxel/block.hpp"
#include "markov/rule.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;

/*
struct Rule {
	std::vector<vec3> relative_positions;
	BlockType result;
};
*/

class MarkovRule {
	public:
		std::vector<Rule> rules;
		MarkovRule();
		MarkovRule(const std::vector<Rule>& initialRules);
		void addRule(const Rule& rule);
		void applyRule(Chunk& C);
		void applyRule(Chunk& C, int limit);

};
