#include "Rules_examples.hpp"


OldRule left_disapear = OldRule(
	{	vec3(0, 0, 0),
		vec3(-1, 0, 0) },
	{	BlockType_Default, 
		BlockType_Empty, 
	},
	BlockType_Empty,
	0.05f
);

OldRule build_base_from_xm = OldRule(
	{ vec3(0, 0, 0),
		vec3(-1, 0, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);

OldRule build_base_from_xp = OldRule(
	{ vec3(0, 0, 0),
		vec3(1, 0, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);


OldRule build_base_from_ym = OldRule(
	{ vec3(0, 0, 0),
		vec3(0, -1, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);

OldRule build_base_from_yp = OldRule(
	{ vec3(0, 0, 0),
		vec3(0, 1, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);


OldRule build_from_under = OldRule(
	{ vec3(0, 0, 0),
		vec3(0, 0, -1) },
	{ BlockType_Empty,
	BlockType_Default },
	BlockType_Default,
	1.0f
);


OldRule appear_OldRule = OldRule(
	{ vec3(0, 0, 0) },
	{ BlockType_Empty },
	BlockType_Default,
	1.0f
);

OldMarkovRule keep_only_full = OldMarkovRule({ left_disapear });
OldMarkovRule build_base = OldMarkovRule({ build_base_from_xm, build_base_from_xp, build_base_from_ym, build_base_from_yp });
OldMarkovRule elevate = OldMarkovRule({ build_from_under });
OldMarkovRule appear = OldMarkovRule({ appear_OldRule });



bool LeftDisappearRule::applies_to(const Chunk& C, const vec3& pos) const {
	if (pos.x <= 0) return false; // Only applies to positions with x > 0
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	bool result = C.m_pBlocks[x - 1][y][z].block_type == BlockType_Empty &&
		C.m_pBlocks[x][y][z].block_type == BlockType_Default;

	return result;
}
void LeftDisappearRule::apply(Chunk& C, const vec3& pos) const {
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Empty; // Set the current block to Empty
}


std::shared_ptr<Rule> leftDisappear = std::make_shared<LeftDisappearRule>();
MarkovRule testLeftDisappear = MarkovRule({ leftDisappear });


bool AppearOnHeadRule::applies_to(const Chunk& C, const vec3& pos) const {
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	if (C.m_pBlocks[x][y][z].block_type == BlockType_Head) {
		vec3 directions[6] = {
		vec3(1, 0, 0), vec3(-1, 0, 0),
		vec3(0, 1, 0), vec3(0, -1, 0),
		vec3(0, 0, 1), vec3(0, 0, -1)
		};
		for (const vec3& dir : directions) {
			vec3 new_pos = pos + dir;
			if (new_pos.x < 0 || new_pos.x >= C.CHUNK_SIZE ||
				new_pos.y < 0 || new_pos.y >= C.CHUNK_SIZE ||
				new_pos.z < 0 || new_pos.z >= C.CHUNK_SIZE) {
				continue; // Out of bounds
			}
			if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Empty) {
				return true; // Found an empty block adjacent to the head
			}
		}
	}
	return false;
}

void AppearOnHeadRule::apply(Chunk& C, const vec3& pos) const {
	// std::cout << "Applying AppearOnHeadRule at position: " << pos << std::endl;
	int MAX_TRIES = 10;
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	int random_idx = rand() % 6; // Randomly choose one of the 6 directions
	for (int i = 0; i < MAX_TRIES; ++i) {
		vec3 directions[6] = {
		vec3(1, 0, 0), vec3(-1, 0, 0),
		vec3(0, 1, 0), vec3(0, -1, 0),
		vec3(0, 0, 1), vec3(0, 0, -1)
		};
		vec3 new_pos = pos + directions[random_idx];
		if (new_pos.x < 0 || new_pos.x >= C.CHUNK_SIZE ||
			new_pos.y < 0 || new_pos.y >= C.CHUNK_SIZE ||
			new_pos.z < 0 || new_pos.z >= C.CHUNK_SIZE) {
			continue;
		}
		C.m_pBlocks[x][y][z].block_type = BlockType_Default; // Set the current block to Default
		C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type = BlockType_Head;

		break;
	}
	
	
}

MarkovRule AppearOnHead = MarkovRule({ std::make_shared<AppearOnHeadRule>() });