// #include "voxel/chunk.hpp"
// #include "voxel/block.hpp"
#pragma once
#include "markov/rule.hpp"
#include <memory>

#include "cgp/cgp.hpp"

using namespace cgp;

class BasicMR {
public:
  std::vector<BasicR> rules;
  BasicMR();
  BasicMR(const std::vector<BasicR> &initialRules);
  void addRule(const BasicR &rule);
  void applyRule(Chunk &C);
  void applyRule(Chunk &C, int limit);
};

class ExtendedMR {
public:
  std::vector<std::shared_ptr<Rule>> rules;

  ExtendedMR() = default;

  ExtendedMR(const std::vector<std::shared_ptr<Rule>> &initialRules)
      : rules(initialRules) {}

  void addRule(const std::shared_ptr<Rule> &rule) { rules.push_back(rule); }

  bool applyRule(Chunk &C);
  bool applyRule(Chunk &C, int limit);
};
