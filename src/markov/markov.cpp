#include "markov.hpp"

// #include "voxel/chunk.hpp"


MarkovRule::MarkovRule() {
	// Constructor implementation
	return;
}

MarkovRule::MarkovRule(const std::vector<Rule>& initialRules) {
	// Constructor implementation
	rules = initialRules;
	return;
}

void MarkovRule::applyRule(const Chunk& C) {
	bool found = true;
	while (found) {
		found = false;
		for (const auto& rule : rules) {
			for (int x = 0; x < C.CHUNK_SIZE; ++x) {
				for (int y = 0; y < C.CHUNK_SIZE; ++y) {
					for (int z = 0; z < C.CHUNK_SIZE; ++z) {
						vec3 pos = { x, y, z };
						if (rule.applies_to(pos)) {
							found = true;
							rule.apply(pos);
						}
					}
				}
			}
		}

	}
}