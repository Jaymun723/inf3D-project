#pragma once

#include "voxel/chunk.hpp"
// #include "voxel/block.hpp"

#include "cgp/cgp.hpp"

using namespace cgp;


class Rule {
public:
    virtual ~Rule() = default;
    virtual bool applies_to(const Chunk& C, const vec3& pos) const = 0;
	virtual void apply(Chunk& C, const vec3& pos) const = 0;
};


class BasicR {
public:
  BasicR(std::vector<vec3> relative_positions,
       std::vector<BlockType> relative_types, BlockType result,
       float probability);
  std::vector<vec3> relative_positions;
  std::vector<BlockType> relative_types;
  BlockType result;
  float probability;
  bool applies_to(const Chunk &C, const vec3 &pos) const;
  bool applies_to(const Chunk &C, const vec3 &pos, const Chunk &Cpx,
                  const Chunk &Cmx, const Chunk &Cpy, const Chunk &Cmy,
                  const Chunk &Cpz, const Chunk &Cmz) const;
  void apply(Chunk &C, const vec3 &pos) const;
};

