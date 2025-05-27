#pragma once

#include "markov/markov.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;

extern OldMarkovRule keep_only_full;
extern OldMarkovRule build_base;
extern OldMarkovRule elevate;
extern OldMarkovRule appear;


class LeftDisappearRule : public Rule {
public:
    bool applies_to(const Chunk& C, const vec3& pos) const override;
    void apply(Chunk& C, const vec3& pos) const override;
};

extern MarkovRule testLeftDisappear;

class AppearOnHeadRule : public Rule {
public:
    bool applies_to(const Chunk& C, const vec3& pos) const override;
    void apply(Chunk& C, const vec3& pos) const override;
};

extern MarkovRule AppearOnHead;