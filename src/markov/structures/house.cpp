#include "house.hpp"
#include <windows.h>

bool BuildGroundRule::applies_to(const Chunk &C, const vec3 &pos) const 
{
	if (pos.x == 0 || pos.x == C.BLOCK_CHUNK_SIZE.x - 1 || pos.y == 0 || pos.y == C.BLOCK_CHUNK_SIZE.y - 1)
	{
		return false; // Don't build on the edges of the chunk
	}
	vec3 directions[8] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0)};

	for (const vec3 &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Plank_Tmp)
		{
			return true;
		}
	}
	return false;
}

void BuildGroundRule::apply(Chunk &C, const vec3 &pos) const
{
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Plank_Tmp;
}

ExtendedMR BuildGroundPlank = ExtendedMR({std::make_shared<BuildGroundRule>()});

bool MakeMiddleDefRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Plank_Tmp)
	{
		return false;
	}
	vec3 directions[8] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0)};
	for (const vec3 &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Grass)
		{
			return false;
		}
	}
	return true;
}

void MakeMiddleDefRule::apply(Chunk &C, const vec3 &pos) const
{
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Plank;
}

ExtendedMR MakeMiddleDef = ExtendedMR({std::make_shared<MakeMiddleDefRule>()});

bool ElevateRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Empty)
	{
		return false;
	}
	if (pos.z > 0 && (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z - 1].block_type == BlockType_Plank_Tmp || C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z - 1].block_type == BlockType_Plank))
	{
		return true;
	}
	return false;
}

void ElevateRule::apply(Chunk &C, const vec3 &pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Plank_Tmp; // Otherwise, just make a plank
}

ExtendedMR Elevate = ExtendedMR({std::make_shared<ElevateRule>()});

bool BuildRoofRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Plank_Tmp && C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Plank)
	{
		return false;
	}
	if (pos.z + 1 <= C.BLOCK_CHUNK_SIZE.z && C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z + 1].block_type != BlockType_Empty)
	{
		return false; // Don't build a roof if the block above is not empty
	}
	vec3 directions[8] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0)};
	for (const vec3 &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type != BlockType_Plank_Tmp && C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type != BlockType_Plank)
		{
			return false;
		}
	}
	return true;
}

void BuildRoofRule::apply(Chunk &C, const vec3 &pos) const
{
	if (pos.z + 1 <= C.BLOCK_CHUNK_SIZE.z)
	{
		C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z + 1].block_type = BlockType_Plank_Tmp; // Set the current block to Plank
	}
}

ExtendedMR BuildRoof = ExtendedMR({std::make_shared<BuildRoofRule>()});

bool BuildFlatRoofRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	if (pos.z >= C.BLOCK_CHUNK_SIZE.z - 1)
	{
		return C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Empty; // Don't build a roof if we are at the top of the chunk
	}
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z + 1].block_type != BlockType_Empty)
	{
		return false;
	}

	vec3 directions[4] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0)};
	int count = 0;
	for (const vec3 &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Plank_Tmp || C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Plank)
		{
			++count;
		}
	}
	return count >= 2;
}

void BuildFlatRoofRule::apply(Chunk &C, const vec3 &pos) const
{
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Flat_Roof; // Set the current block to Plank
}

ExtendedMR BuildFlatRoof = ExtendedMR({std::make_shared<BuildFlatRoofRule>()});

bool MakeExtDefRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Plank_Tmp)
	{
		return false;
	}
	vec3 directions[8] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0)};
	for (const vec3 &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type != BlockType_Plank && C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type != BlockType_Plank_Tmp)
		{
			return true;
		}
	}
	return false;
}

void MakeExtDefRule::apply(Chunk &C, const vec3 &pos) const
{
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Plank; // Change the temporary plank to normal plank
}

ExtendedMR MakeExtDef = ExtendedMR({std::make_shared<MakeExtDefRule>()});

bool RemovePlankTmpRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Plank_Tmp);
}

void RemovePlankTmpRule::apply(Chunk &C, const vec3 &pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Empty;
}

ExtendedMR RemovePlankTmp = ExtendedMR({std::make_shared<RemovePlankTmpRule>()});

bool AppearNewGroundRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Flat_Roof);
}

void AppearNewGroundRule::apply(Chunk &C, const vec3 &pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;																				 // Place the new ground block above the flat roof
	C.m_pBlocks[x][y][z].block_type = BlockType_Plank_Tmp; // Change the flat roof to temporary grass
}

ExtendedMR AppearNewGround = ExtendedMR({std::make_shared<AppearNewGroundRule>()});

bool CleanSingleBlocksRule::applies_to(const Chunk &C, const vec3 &pos) const
{
	if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Empty) {
		return false;
	}
	vec3 directions[4] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0)};
	for (const vec3 &dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type != BlockType_Empty)
		{
			return false; // If any adjacent block is not empty, don't clean
		}
	}
	return true;
}

void CleanSingleBlocksRule::apply(Chunk &C, const vec3 &pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Empty; // Set the current block to empty
}

ExtendedMR CleanSingleBlocks = ExtendedMR({std::make_shared<CleanSingleBlocksRule>()});


bool BuildPillarsRule::applies_to(const Chunk& C, const vec3& pos) const
{
	int x = (int)pos.x;
	int y = (int)pos.y;
	int z = (int)pos.z;
	if (C.m_pBlocks[x][y][z].block_type != BlockType_Empty) {
		return false;
	}
	if (z + 1 == C.BLOCK_CHUNK_SIZE.z) {
		return false;
	}
	if (C.m_pBlocks[x][y][z + 1].block_type == BlockType_Pillar) {
		return true;
	}
	if (C.m_pBlocks[x][y][z + 1].block_type != BlockType_Plank
		&& C.m_pBlocks[x][y][z + 1].block_type != BlockType_Flat_Roof) {
		return false;
	}
	int count = 0;
	vec3 directions[4] = { vec3(1, 0, 1), vec3(-1, 0, 1), vec3(0, 1, 1), vec3(0, -1, 1) };
	for (const vec3& dir : directions)
	{
		vec3 new_pos = pos + dir;
		if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 || new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
		{
			continue;
		}
		if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Plank
			|| C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Flat_Roof)
		{
			++count;
		}
	}
	return count == 2;
}

void BuildPillarsRule::apply(Chunk& C, const vec3& pos) const
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;
	C.m_pBlocks[x][y][z].block_type = BlockType_Pillar;
}

ExtendedMR BuildPillar = ExtendedMR({ std::make_shared<BuildPillarsRule>() });


int build_house_aux(Chunk &C, int step)
{
	int x, y, z;
	switch (step)
	{
	case 0:
		C.m_pBlocks[0][0][0].block_type = BlockType_Grass;
		step = 1;
		break;

	case 1:
		if (BuildGroundGrass.applyRule(C, 5))
		{
			break;
		}
		step = 2;
		break;

	case 2:
		x = C.BLOCK_CHUNK_SIZE.x / 4 + rand() % (C.BLOCK_CHUNK_SIZE.x / 2);
		y = C.BLOCK_CHUNK_SIZE.y / 4 + rand() % (C.BLOCK_CHUNK_SIZE.y / 2);
		z = 0;
		C.m_pBlocks[x][y][z].block_type = BlockType_Plank_Tmp;
		step = 3;
		break;

	case 3:
		BuildGroundPlank.applyRule(C, -1);
		step = 4;
		break;

	case 4:
		if (BuildGroundPlank.applyRule(C, -1))
		{
			if (rand() % 4 == 0)
			{
				step = 5;
			}
			break;
		}
		step = 5;
		break;

	case 5:
		if (MakeMiddleDef.applyRule(C, -1))
		{
			break;
		}
		step = 6;
		break;

	case 6:
		if (Elevate.applyRule(C, -1))
		{
			if (rand() % 6 == 0)
			{
				step = 8;
			}
			break;
		}
		step = 8;
		break;

	case 7:
		if (BuildRoof.applyRule(C, -1))
		{
			break;
		}
		step = 9;
		break;

	case 8:
		if (BuildFlatRoof.applyRule(C, -1))
		{
			step = 9;
			break;
		}

	case 9:
		if (MakeExtDef.applyRule(C, -1))
		{
			break;
		}
		step = 10;
		break;

	case 10:
		if (RemovePlankTmp.applyRule(C, -1))
		{
			break;
		}
		step = 11;
		break;

	case 11:
		if (rand() % 6 == 0)
		{
			step = 12;
		}
		else
		{
			AppearNewGround.applyRule(C, 1);
			step = 3;
		}
		break;

	case 12:

		if (CleanSingleBlocks.applyRule(C, -1))
		{
			break;
		}
		step = 13;
		break;

	case 13:
		if (BuildPillar.applyRule(C, -1))
		{
			break;
		}
		step = -1;
		break;

	case -1:
		return -1;

	default:
		std::cout << "Invalid step in build_house_aux: " << step << std::endl;
		break;

	}
	return step;
}

int build_house(Chunk &C, int step, int speed)
{
	for (int i = 0; i < speed; ++i)
	{
		step = build_house_aux(C, step);
	}
	return step;
}