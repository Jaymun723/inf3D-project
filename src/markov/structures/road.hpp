#pragma once
#include "markov/markov.hpp"
#include "markov/rules_examples.hpp"
#include "cgp/cgp.hpp"

using namespace cgp;


class BuildBasicRoadRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};

class MakeXSidewalkRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};

class MakeYSidewalkRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class MakeCrossroadSideWalkRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};


class ExtendXWhiteLineRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};

class ExtendYWhiteLineRule : public Rule {
public:
	bool applies_to(const Chunk& C, const vec3& pos) const override;
	void apply(Chunk& C, const vec3& pos) const override;
};



int build_Xroad(Chunk& C, int step, int speed);


int build_Yroad(Chunk& C, int step, int speed);


int build_Crossroad(Chunk& C, int step, int speed);