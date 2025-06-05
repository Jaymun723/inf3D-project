#pragma once
#include "markov/markov.hpp"
#include "markov/rules_examples.hpp"
#include "cgp/cgp.hpp"

using namespace cgp;

class BuildBlackChunkRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class MakeMazeRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class AppearWhiteRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class RemoveBlackRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class WhitenGreyRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};

int build_maze(Chunk& C, int step, int speed);