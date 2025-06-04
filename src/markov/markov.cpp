#include "markov.hpp"
#include <algorithm>
// #include "voxel/chunk.hpp"

BasicMR::BasicMR() { rules = {}; }

BasicMR::BasicMR(const std::vector<BasicR> &initialRules)
    : rules(std::move(initialRules)) {}

void BasicMR::addRule(const BasicR &rule) { rules.push_back(rule); }


std::vector<int> generate_random_indices(int size, int count) {
	std::vector<int> indices(size);
	for (int i = 0; i < size; ++i) {
		indices[i] = i;
	}
	std::random_shuffle(indices.begin(), indices.end());
	indices.resize(count);   
    return indices;
}


void BasicMR::applyRule(Chunk &C, int limit) {
  bool found = true;
  int i = 0;
  while (found && (i < limit || limit < 0)) {
    found = false;
    for (const auto &rule : rules) {
      for (int x = 0; x < C.CHUNK_SIZE; ++x) {
	    std::vector<int> rx = generate_random_indices(C.CHUNK_SIZE, C.CHUNK_SIZE);
        for (int y = 0; y < C.CHUNK_SIZE; ++y) {
          std::vector<int> ry = generate_random_indices(C.CHUNK_SIZE, C.CHUNK_SIZE);
          for (int z = 0; z < C.CHUNK_SIZE; ++z) {
            std::vector<int> rz = generate_random_indices(C.CHUNK_SIZE, C.CHUNK_SIZE);
            vec3 pos = {(int)rx[x], (int)ry[y], (int)rz[z]};
            std::cout << pos << std::endl;
            if (i < limit || limit < 0) {
              if (rule.applies_to(C, pos)) {
                found = true;
                rule.apply(C, pos);
                ++i;
              }
            }
          }
        }
      }
    }
  }
  C.Load();
}

void BasicMR::applyRule(Chunk &C) { applyRule(C, -1); }


bool ExtendedMR::applyRule(Chunk& C, int limit) {
    bool found = true;
    int i = 0;
    while (found && (i < limit || limit < 0)) {
        found = false;
        for (const auto& rule : rules) {
			std::vector<int> positions = generate_random_indices(C.CHUNK_SIZE * C.CHUNK_SIZE * C.CHUNK_SIZE, C.CHUNK_SIZE * C.CHUNK_SIZE * C.CHUNK_SIZE);
            for (int pos_index = 0; pos_index < positions.size(); ++pos_index) {
                int z = positions[pos_index] / (C.CHUNK_SIZE * C.CHUNK_SIZE);
                int y = (positions[pos_index] / C.CHUNK_SIZE) % C.CHUNK_SIZE;
                int x = positions[pos_index] % C.CHUNK_SIZE;
                vec3 pos = { x, y, z };
                if (i < limit || limit < 0) {
                    if (rule->applies_to(C, pos)) {
                        found = true;
                        rule->apply(C, pos);
                        ++i;
                    }
                }
                else {
                    break;
                }
            }
        }
    }
    return (i > 0);
}


bool ExtendedMR::applyRule(Chunk& C) { return applyRule(C, -1); }
