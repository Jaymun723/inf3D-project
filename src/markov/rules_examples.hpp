#pragma once

#include "markov/markov.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;

extern BasicMR keep_only_full;
extern BasicMR build_base;
extern BasicMR elevate;
extern BasicMR appear;


class LeftDisappearRule : public Rule {
public:
    bool applies_to(const Chunk& C, const vec3& pos) const override;
    void apply(Chunk& C, const vec3& pos) const override;
};

extern ExtendedMR testLeftDisappear;


class BuildGroundGrassRule : public Rule {
public:
    bool applies_to(const Chunk& C, const vec3& pos) const override;
    void apply(Chunk& C, const vec3& pos) const override;
};


class BuildGroundGrassTmpRule : public Rule {
public:
    bool applies_to(const Chunk& C, const vec3& pos) const override;
    void apply(Chunk& C, const vec3& pos) const override;
};


extern ExtendedMR BuildGroundGrass;
extern ExtendedMR BuildGroundGrassTmp;
