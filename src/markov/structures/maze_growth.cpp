#include "maze_growth.hpp"
#include <algorithm>

bool BuildBlackChunkRule::applies_to(const Chunk& C, const vec3& pos) const {
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Empty) {
		return false;
	}
	return true;
}

void BuildBlackChunkRule::apply(Chunk& C, const vec3& pos) const {
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Black; // Set the current block to black
}

ExtendedMR BuildBlackChunk = ExtendedMR({ std::make_shared<BuildBlackChunkRule>() });


bool MakeMazeRule::applies_to(const Chunk& C, const vec3& pos) const {
	int x = (int)pos.x;
	int y = (int)pos.y;
	int z = (int)pos.z;
	if (C.m_pBlocks[x][y][z].block_type != BlockType_White) {
		return false;
	}
	vec3 directions1[6] = { vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1) };
	vec3 directions2[6] = { vec3(2, 0, 0), vec3(-2, 0, 0), vec3(0, 2, 0), vec3(0, -2, 0) , vec3(0, 0, 2), vec3(0, 0, -2) };
	for (int ri = 0; ri < 6; ++ri) {
		vec3 pos1 = pos + directions1[ri];
		vec3 pos2 = pos + directions2[ri];

		if (pos1.x < 0 || pos1.x >= C.CHUNK_SIZE || pos1.y < 0 || pos1.y >= C.CHUNK_SIZE || pos1.z < 0 || pos1.z >= C.CHUNK_SIZE) {
			continue; // Out of bounds
		}
		if (pos2.x < 0 || pos2.x >= C.CHUNK_SIZE || pos2.y < 0 || pos2.y >= C.CHUNK_SIZE || pos2.z < 0 || pos2.z >= C.CHUNK_SIZE) {
			continue; // Out of bounds
		}
		if (C.m_pBlocks[(int)pos1.x][(int)pos1.y][(int)pos1.z].block_type != BlockType_Black
			|| C.m_pBlocks[(int)pos2.x][(int)pos2.y][(int)pos2.z].block_type != BlockType_Black) {
			continue; // Both positions must be black
		}
		return true;
	}
	return false;
}


void MakeMazeRule::apply(Chunk& C, const vec3& pos) const {
	vec3 directions1[6] = { vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};
	vec3 directions2[6] = { vec3(2, 0, 0), vec3(-2, 0, 0), vec3(0, 2, 0), vec3(0, -2, 0) , vec3(0, 0, 2), vec3(0, 0, -2)};
	std::vector<int> ridx = generate_random_indices(6, 6); // Randomly shuffle the directions
	for (int ri = 0; ri < 6; ++ri) {
		vec3 pos1 = pos + directions1[ridx[ri]];
		vec3 pos2 = pos + directions2[ridx[ri]];

		if (pos1.x < 0 || pos1.x >= C.CHUNK_SIZE || pos1.y < 0 || pos1.y >= C.CHUNK_SIZE || pos1.z < 0 || pos1.z >= C.CHUNK_SIZE) {
			continue; // Out of bounds
		}
		if (pos2.x < 0 || pos2.x >= C.CHUNK_SIZE || pos2.y < 0 || pos2.y >= C.CHUNK_SIZE || pos2.z < 0 || pos2.z >= C.CHUNK_SIZE) {
			continue; // Out of bounds
		}
		if (C.m_pBlocks[(int)pos1.x][(int)pos1.y][(int)pos1.z].block_type != BlockType_Black
			|| C.m_pBlocks[(int)pos2.x][(int)pos2.y][(int)pos2.z].block_type != BlockType_Black) {
			continue; // Both positions must be black
		}
		C.m_pBlocks[(int)pos1.x][(int)pos1.y][(int)pos1.z].block_type = BlockType_Grey;
		C.m_pBlocks[(int)pos2.x][(int)pos2.y][(int)pos2.z].block_type = BlockType_White;
		break;
	}
}

ExtendedMR MakeMaze = ExtendedMR({ std::make_shared<MakeMazeRule>() });


bool RemoveBlackRule::applies_to(const Chunk& C, const vec3& pos) const {
	return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Black);
}

void RemoveBlackRule::apply(Chunk& C, const vec3& pos) const {
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Empty; // Set the current block to empty
}

bool WhitenGreyRule::applies_to(const Chunk& C, const vec3& pos) const {
	return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Grey);
}


void WhitenGreyRule::apply(Chunk& C, const vec3& pos) const {
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_White; // Set the current block to empty
}


ExtendedMR CleanMaze = ExtendedMR({ std::make_shared<RemoveBlackRule>(), std::make_shared<WhitenGreyRule>() });


bool AppearWhiteRule::applies_to(const Chunk& C, const vec3& pos) const {
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Black) {
		return false;
	}
	return true;
}


void AppearWhiteRule::apply(Chunk& C, const vec3& pos) const {
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_White; // Set the current block to white
}

ExtendedMR AppearWhite = ExtendedMR({ std::make_shared<AppearWhiteRule>() });


int build_maze_aux(Chunk& C, int step) {
	switch (step) {
	case 0:
		if (BuildBlackChunk.applyRule(C, 1 + rand()%100)) {
			if (rand() % 100 == 0) {
				step = 1;
			}
			break;
		}
		step = 2;
		break;

	case 1:
		BuildBlackChunk.applyRule(C, -1);
		step = 2;
		break;

	case 2:
		AppearWhite.applyRule(C, 1);
		step = 3;
		break;

	case 3:
		if (MakeMaze.applyRule(C, -1)) {
			break;
		}
		step = 4;
		break;

	case 4:
		if (CleanMaze.applyRule(C, 20 + rand()%100)) {
			break;
		}
		step = 5;
		break;

	default:
		break;
	}
	return step;
}

int build_maze(Chunk& C, int step, int speed) {
	for (int i = 0; i < speed; ++i) {
		step = build_maze_aux(C, step);
	}
	C.UpdateMesh();
	return step;
}