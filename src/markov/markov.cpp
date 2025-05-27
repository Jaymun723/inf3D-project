#include "markov.hpp"

// #include "voxel/chunk.hpp"

MarkovRule::MarkovRule() { rules = {}; }

MarkovRule::MarkovRule(const std::vector<Rule> &initialRules)
    : rules(std::move(initialRules)) {}

void MarkovRule::addRule(const Rule &rule) { rules.push_back(rule); }

void MarkovRule::applyRule(Chunk &C, int limit) {
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

void MarkovRule::applyRule(Chunk &C) { applyRule(C, -1); }