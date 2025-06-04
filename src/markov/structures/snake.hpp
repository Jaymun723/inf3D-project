#pragma once
#include "markov/markov.hpp"
#include "markov/rules_examples.hpp"
#include "cgp/cgp.hpp"

using namespace cgp;


class AppearOnHeadRule : public Rule {
public:
    bool applies_to(const Chunk& C, const vec3& pos) const override;
    void apply(Chunk& C, const vec3& pos) const override;
};

extern ExtendedMR AppearOnHead;


int build_snake(Chunk& C, int step, int speed);