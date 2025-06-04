#include "rules_examples.hpp"

BasicR left_disapear = BasicR({vec3(0, 0, 0), vec3(-1, 0, 0)},
                                {
                                    BlockType_Default,
                                    BlockType_Empty,
                                },
                                BlockType_Empty, 0.05f);

BasicR build_base_from_xm =
    BasicR({vec3(0, 0, 0), vec3(-1, 0, 0)},
            {BlockType_Empty, BlockType_Default}, BlockType_Default, 1.0f);

BasicR build_base_from_xp =
    BasicR({vec3(0, 0, 0), vec3(1, 0, 0)},
            {BlockType_Empty, BlockType_Default}, BlockType_Default, 1.0f);

BasicR build_base_from_ym =
    BasicR({vec3(0, 0, 0), vec3(0, -1, 0)},
            {BlockType_Empty, BlockType_Default}, BlockType_Default, 1.0f);

BasicR build_base_from_yp =
    BasicR({vec3(0, 0, 0), vec3(0, 1, 0)},
            {BlockType_Empty, BlockType_Default}, BlockType_Default, 1.0f);

BasicR build_from_under =
    BasicR({vec3(0, 0, 0), vec3(0, 0, -1)},
            {BlockType_Empty, BlockType_Default}, BlockType_Default, 1.0f);

BasicR appear_OldRule =
    BasicR({vec3(0, 0, 0)}, {BlockType_Empty}, BlockType_Default, 1.0f);

BasicMR keep_only_full = BasicMR({left_disapear});
BasicMR build_base =
    BasicMR({build_base_from_xm, build_base_from_xp, build_base_from_ym,
                   build_base_from_yp});
BasicMR elevate = BasicMR({build_from_under});
BasicMR appear = BasicMR({appear_OldRule});


bool general_BuildGround_rule_applies_to(const Chunk& C, const vec3& pos, BlockType type) {
    if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Empty) {
        return false;
    }
    vec3 directions[6] = { vec3(1, 0, 0),  vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0)};
    for (const vec3& dir : directions) {
        vec3 new_pos = pos + dir;
        if (new_pos.x < 0 || new_pos.x >= C.CHUNK_SIZE ||
            new_pos.y < 0 || new_pos.y >= C.CHUNK_SIZE ||
            new_pos.z < 0 || new_pos.z >= C.CHUNK_SIZE) {
            continue;
        }
        if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == type) {
            return true;
        }
    }
    return false;
}
void general_BuildGround_rule_apply(Chunk& C, const vec3& pos, BlockType type) {
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    C.m_pBlocks[x][y][z].block_type = type;
}



bool LeftDisappearRule::applies_to(const Chunk &C, const vec3 &pos) const {
  if (pos.x <= 0)
    return false; // Only applies to positions with x > 0
  int x = pos.x;
  int y = pos.y;
  int z = pos.z;
  bool result = C.m_pBlocks[x - 1][y][z].block_type == BlockType_Empty &&
                C.m_pBlocks[x][y][z].block_type == BlockType_Default;

  return result;
}
void LeftDisappearRule::apply(Chunk &C, const vec3 &pos) const {
  int x = pos.x;
  int y = pos.y;
  int z = pos.z;
  C.m_pBlocks[x][y][z].block_type =
      BlockType_Empty; // Set the current block to Empty
}

std::shared_ptr<Rule> leftDisappear = std::make_shared<LeftDisappearRule>();
ExtendedMR testLeftDisappear = ExtendedMR({leftDisappear});


bool BuildGroundGrassRule::applies_to(const Chunk& C, const vec3& pos) const {
	return general_BuildGround_rule_applies_to(C, pos, BlockType_Grass);
}

void BuildGroundGrassRule::apply(Chunk& C, const vec3& pos) const {
	general_BuildGround_rule_apply(C, pos, BlockType_Grass);
}

ExtendedMR BuildGroundGrass = ExtendedMR({ std::make_shared<BuildGroundGrassRule>() });



bool BuildGroundGrassTmpRule::applies_to(const Chunk& C, const vec3& pos) const {
    return general_BuildGround_rule_applies_to(C, pos, BlockType_Grass_Tmp);
}

void BuildGroundGrassTmpRule::apply(Chunk& C, const vec3& pos) const {
    general_BuildGround_rule_apply(C, pos, BlockType_Grass_Tmp);
}

ExtendedMR BuildGroundGrassTmp = ExtendedMR({ std::make_shared<BuildGroundGrassTmpRule>() });

