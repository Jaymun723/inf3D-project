#pragma once
#include "markov/markov.hpp"
#include "markov/rules_examples.hpp"
#include "cgp/cgp.hpp"

using namespace cgp;


class BuildGroundRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class MakeMiddleDefRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class ElevateRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class BuildRoofRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class MakeExtDefRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class RemovePlankTmpRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};

int build_house(Chunk& C, int step, int speed);

