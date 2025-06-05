#include "river.hpp"

bool BuildRiverRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Grass_Tmp)
	{
		return false;
	}
	vec3 directions[8] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0)};

	for (auto &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x ||
				new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y ||
				new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Grass)
		{
			return true;
		}
	}
	return false;
}

void BuildRiverRule::apply(Chunk &C, const vec3 &pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Water; // Change the temporary grass to normal grass
	// Randomly change adjacent blocks to river blocks
	vec3 directions[8] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0)};
	for (const vec3 &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x ||
				new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y ||
				new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type = BlockType_Water;
	}
}

ExtendedMR BuildRiver = ExtendedMR({std::make_shared<BuildRiverRule>()});

bool MakeGrassTmpRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Grass);
}

void MakeGrassTmpRule::apply(Chunk &C, const vec3 &pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Grass_Tmp;
}

ExtendedMR MakeGrassTmp = ExtendedMR({std::make_shared<MakeGrassTmpRule>()});


bool ElevateGrassRule::applies_to(const Chunk& C, const vec3& pos) const {
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Grass_Tmp) {
		return false;
	}
	if (pos.z + 1 >= C.BLOCK_CHUNK_SIZE.z) {
		return false;
	}
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z + 1].block_type != BlockType_Empty) {
		return false;
	}
	vec3 directions[24] = { 
		vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
		vec3(2, 0, 0), vec3(2, 1, 0), vec3(2, 2, 0), vec3(1, 2, 0), vec3(0, 2, 0), vec3(-1, 2, 0), vec3(-2, 2, 0), vec3(-2, 1, 0), vec3(-2, 0, 0), vec3(-2, -1, 0), vec3(-2, -2, 0), vec3(-1, -2, 0), vec3(0, -2, 0), vec3(1, -2, 0), vec3(2, -2, 0), vec3(2, -1, 0)};
	int count = 0;
	for (const vec3& dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x ||
			new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y ||
			new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			++count;
			continue; // Out of bounds
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Grass
			|| C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Grass_Tmp) {
			++count;
		}
	}
	return count > 20;
}

void ElevateGrassRule::apply(Chunk& C, const vec3& pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Grass;
	vec3 directions[24] = {
		vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
		vec3(2, 0, 0), vec3(2, 1, 0), vec3(2, 2, 0), vec3(1, 2, 0), vec3(0, 2, 0), vec3(-1, 2, 0), vec3(-2, 2, 0), vec3(-2, 1, 0), vec3(-2, 0, 0), vec3(-2, -1, 0), vec3(-2, -2, 0), vec3(-1, -2, 0), vec3(0, -2, 0), vec3(1, -2, 0), vec3(2, -2, 0), vec3(2, -1, 0) };
	int count = 0;
	for (const vec3& dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x ||
			new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y ||
			new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			++count;
			continue; // Out of bounds
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Grass
			|| C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Grass_Tmp) {
			++count;
		}
	}
	if (20 + rand() % 4 < count) {
		C.m_pBlocks[x][y][z+1].block_type = BlockType_Grass_Tmp;
	}
}

ExtendedMR ElevateGrass = ExtendedMR({ std::make_shared<ElevateGrassRule>() });



bool RemoveGrassTmpRule::applies_to(const Chunk& C, const vec3& pos) const
{
	return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Grass_Tmp);
}

void RemoveGrassTmpRule::apply(Chunk& C, const vec3& pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Grass;
}

ExtendedMR RemoveGrassTmp = ExtendedMR({ std::make_shared<RemoveGrassTmpRule>() });


int build_river_aux(Chunk &C, int step)
{
	bool found_normal, found_tmp;
	switch (step)
	{
	case 0:

		C.m_pBlocks[rand() % C.BLOCK_CHUNK_SIZE.x][rand() % C.BLOCK_CHUNK_SIZE.y][0].block_type = BlockType_Grass;
		C.m_pBlocks[rand() % C.BLOCK_CHUNK_SIZE.x][rand() % C.BLOCK_CHUNK_SIZE.y][0].block_type = BlockType_Grass_Tmp;

		step = 1;
		break;

	case 1:
		found_normal = BuildGroundGrass.applyRule(C, 1);
		found_tmp = BuildGroundGrassTmp.applyRule(C, 1);
		if (found_normal || found_tmp)
		{
			break;
		}
		step = 2;
		break;

	case 2:
		if (BuildRiver.applyRule(C, 1))
		{
			break;
		}
		step = 3;
		break;

	case 3:
		if (MakeGrassTmp.applyRule(C, 5))
		{
			break;
		}
		step = 4;
		break;

	case 4:
		if (ElevateGrass.applyRule(C, -1))
		{
			break;
		}
		step = 5;
		break;

	case 5:
		if (RemoveGrassTmp.applyRule(C, 5)) {
			break;
		}
		step = 6;
		break;

	default:
		return step;
	}

	return step;
}

int build_river(Chunk &C, int step, int speed)
{
	for (int i = 0; i < speed; ++i)
	{
		step = build_river_aux(C, step);
	}
	C.UpdateMesh();
	return step;
}