// #include "voxel/chunk.hpp"
// #include "voxel/block.hpp"
#pragma once
#include "markov/rule.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;


class OldMarkovRule {
public:
  std::vector<OldRule> rules;
  OldMarkovRule();
  OldMarkovRule(const std::vector<OldRule> &initialRules);
  void addRule(const OldRule &rule);
  void applyRule(Chunk &C);
  void applyRule(Chunk &C, int limit);
};


class MarkovRule {
public:
    std::vector<std::shared_ptr<Rule>> rules;

    MarkovRule() = default;

    MarkovRule(const std::vector<std::shared_ptr<Rule>>& initialRules)
        : rules(initialRules) {
    }

    void addRule(const std::shared_ptr<Rule>& rule) {
        rules.push_back(rule);
    }

    bool applyRule(Chunk& C);
    bool applyRule(Chunk& C, int limit);
};
