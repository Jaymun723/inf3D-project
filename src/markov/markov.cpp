#include "markov.hpp"

// #include "voxel/chunk.hpp"

BasicMR::BasicMR() { rules = {}; }

BasicMR::BasicMR(const std::vector<BasicR> &initialRules)
    : rules(std::move(initialRules)) {}

void BasicMR::addRule(const BasicR &rule) { rules.push_back(rule); }

void BasicMR::applyRule(Chunk &C, int limit) {
  bool found = true;
  int i = 0;
  while (found && (i < limit || limit < 0)) {
    found = false;
    for (const auto &rule : rules) {
      for (int x = 0; x < C.CHUNK_SIZE; ++x) {
        for (int y = 0; y < C.CHUNK_SIZE; ++y) {
          for (int z = 0; z < C.CHUNK_SIZE; ++z) {
            vec3 pos = {x, y, z};
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
            for (int z = 0; z < C.CHUNK_SIZE; ++z) {
                for (int y = 0; y < C.CHUNK_SIZE; ++y) {
                    for (int x = 0; x < C.CHUNK_SIZE; ++x) {
                        vec3 pos = { x, y, z };
                        if (i < limit || limit < 0) {
                            if (rule->applies_to(C, pos)) {
                                found = true;
                                rule->apply(C, pos);
                                ++i;
								// std::cout << "Applied rule at position: " << pos << std::endl;
                            }
                        }
                        else {
                            break;
                        }
                    }
					if (i >= limit && limit >= 0) {
						break;
					}
                }
                if (i >= limit && limit >= 0) {
                    break;
                }
            }
            if (i >= limit && limit >= 0) {
                break;
            }
        }
    }
    return (i > 0);
}


bool ExtendedMR::applyRule(Chunk& C) { return applyRule(C, -1); }
