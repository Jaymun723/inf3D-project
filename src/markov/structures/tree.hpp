#pragma once
#include "markov/markov.hpp"
#include "markov/rules_examples.hpp"
#include "cgp/cgp.hpp"

using namespace cgp;


class ExtendTrunkRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};

class ExtendLeafRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};

int build_tree(Chunk& C, int step, int speed);