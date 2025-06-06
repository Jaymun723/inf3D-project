#include "road.hpp"


bool BuildBasicRoadRule::applies_to(const Chunk& C, const vec3& pos) const {
	if (pos.x == 0 || C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Empty) {
		return false;
	}
	return (C.m_pBlocks[(int)pos.x - 1][(int)pos.y][(int)pos.z].block_type == BlockType_Road_Tmp
		|| C.m_pBlocks[(int)pos.x - 1][(int)pos.y][(int)pos.z].block_type == BlockType_Road);
}

void BuildBasicRoadRule::apply(Chunk& C, const vec3& pos) const {
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Road_Tmp; // Set the current block to Road
}

ExtendedMR BuildBasicXRoad = ExtendedMR({ std::make_shared<BuildBasicRoadRule>() });


bool MakeXSidewalkRule::applies_to(const Chunk& C, const vec3& pos) const {
	int size = C.BLOCK_CHUNK_SIZE.y / 8;
	int x = (int)pos.x;
	int y = (int)pos.y;
	int z = (int)pos.z;
	if (C.m_pBlocks[x][y][z].block_type != BlockType_Road_Tmp && C.m_pBlocks[x][y][z].block_type != BlockType_Road) {
		return false;
	}
	if (y < size || y >= C.BLOCK_CHUNK_SIZE.y - size) {
		return true;
	}
	return false;
}


void MakeXSidewalkRule::apply(Chunk& C, const vec3& pos) const {
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_Sidewalk;
}

ExtendedMR MakeXSidewalk = ExtendedMR({ std::make_shared<MakeXSidewalkRule>() });

bool ExtendXWhiteLineRule::applies_to(const Chunk& C, const vec3& pos) const {
	int x = (int)pos.x;
	int y = (int)pos.y;
	int z = (int)pos.z;
	if (C.m_pBlocks[x][y][z].block_type != BlockType_Road 
		&& C.m_pBlocks[x][y][z].block_type != BlockType_Road_Tmp) {
		return false;
	}
	if (x < 2) {
		return false;
	}
	if (x < 3) {
		return C.m_pBlocks[1][y][z].block_type == BlockType_White_Line_Tmp;
	}
	if (x < 4) {
		return false;
	}
	int count = 0;
	for (int dx = 1; dx <= 4; ++dx) {
		if (C.m_pBlocks[x - dx][y][z].block_type == BlockType_White_Line_Tmp) {
			count++;
		}
	}
	return count == 2 && C.m_pBlocks[x-4][y][z].block_type == BlockType_White_Line_Tmp;
}


void ExtendXWhiteLineRule::apply(Chunk& C, const vec3& pos) const {
	C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type = BlockType_White_Line_Tmp;
}

ExtendedMR ExtendXWhiteLine = ExtendedMR({ std::make_shared<ExtendXWhiteLineRule>() });


int build_Xroad_aux(Chunk& C, int step) {
	int x, y, z;
	switch (step)
	{
	case 0:
		for (int y = 0; y < C.BLOCK_CHUNK_SIZE.y; ++y) {
			C.m_pBlocks[0][y][0].block_type = BlockType_Road_Tmp;
		}
		step = 1;
	case 1:
		if (BuildBasicXRoad.applyRule(C, -1)) {
			break;
		}
		step = 2;
		break;

	case 2:
		if (MakeXSidewalk.applyRule(C, -1)) {
			break;
		}
		step = 3;
		break;

	case 3:
		y = C.BLOCK_CHUNK_SIZE.y / 2;
		C.m_pBlocks[1][y][0].block_type = BlockType_White_Line_Tmp;
		step = 4;
		break;

	case 4:
		if (ExtendXWhiteLine.applyRule(C, -1)) {
			break;
		}
		step = -1;
		break;


	case -1:
		return -1;

	default:
		std::cout << "Invalid step in build_road_aux: " << step << std::endl;
		break;


	}

	return step;
}


int build_Xroad(Chunk& C, int step, int speed) {
	for (int i = 0; i < speed; ++i) {
		step = build_Xroad_aux(C, step);
	}
	return step;
}