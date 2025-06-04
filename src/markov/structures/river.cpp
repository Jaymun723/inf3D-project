#include "river.hpp"


bool BuildRiverRule::applies_to(const Chunk& C, const vec3& pos) const {
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Grass_Tmp) {
		return false;
	}
	vec3 directions[8] = { vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0) };

	for (auto& dir : directions) {
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.CHUNK_SIZE ||
			new_pos.y < 0 || new_pos.y >= C.CHUNK_SIZE ||
			new_pos.z < 0 || new_pos.z >= C.CHUNK_SIZE) {
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Grass) {
			return true;
		}
	}
	return false;
}


void BuildRiverRule::apply(Chunk& C, const vec3& pos) const {
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Water; // Change the temporary grass to normal grass
	// Randomly change adjacent blocks to river blocks
	vec3 directions[8] = { vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0)};
	for (const vec3& dir : directions) {
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.CHUNK_SIZE ||
			new_pos.y < 0 || new_pos.y >= C.CHUNK_SIZE ||
			new_pos.z < 0 || new_pos.z >= C.CHUNK_SIZE) {
			continue;
		}
		C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type = BlockType_Water;
	}
}

ExtendedMR BuildRiver = ExtendedMR({ std::make_shared<BuildRiverRule>() });


bool RemoveGrassTmpRule::applies_to(const Chunk& C, const vec3& pos) const {
	return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Grass_Tmp);
}

void RemoveGrassTmpRule::apply(Chunk& C, const vec3& pos) const {
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Grass;
}

ExtendedMR RemoveGrassTmp = ExtendedMR({ std::make_shared<RemoveGrassTmpRule>() });

int build_river_aux(Chunk& C, int step) {
	bool found_normal, found_tmp;
	switch (step) {
	case 0:

		C.m_pBlocks[rand()%C.CHUNK_SIZE][rand() % C.CHUNK_SIZE][0].block_type = BlockType_Grass;
		C.m_pBlocks[rand() % C.CHUNK_SIZE][rand() % C.CHUNK_SIZE][0].block_type = BlockType_Grass_Tmp;

		step = 1;
		break;

	case 1:
		found_normal = BuildGroundGrass.applyRule(C, 1);
		found_tmp = BuildGroundGrassTmp.applyRule(C, 1);
		if (found_normal || found_tmp) {
			break;
		}
		step = 2;
		break;

	case 2:
		if (BuildRiver.applyRule(C, 1)) {
			break;
		}
		step = 3;
		break;

	case 3:
		if (RemoveGrassTmp.applyRule(C, 1)) {
			break;
		}
		step = 4;
		break;

	default:
		return step;
	}

	return step;
}

int build_river(Chunk& C, int step, int speed) {
	for (int i = 0; i < speed; ++i) {
		step = build_river_aux(C, step);
	}
	C.UpdateMesh();
	return step;
}