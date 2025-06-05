#pragma once

#include "cgp/cgp.hpp"

enum BlockType
{
	BlockType_Empty,
	BlockType_Default,
	BlockType_Head,
	BlockType_Grass_Tmp,
	BlockType_Grass,
	BlockType_Wood_Tmp,
	BlockType_Wood,
	BlockType_Leaf_Tmp,
	BlockType_Leaf,
	BlockType_Rainbow,
	BlockType_Water,
	BlockType_Plank_Tmp,
	BlockType_Plank,
	BlockType_Flat_Roof,
	BlockType_Road_Tmp,
	BlockType_Road,
	BlockType_White_Line_Tmp,
	BlockType_White_Line,
	BlockType_Sidewalk,
	BlockType_Black,
	BlockType_Grey,
	BlockType_White
};

class Block
{
	/**
	 * @brief Défini un block dans notre monde.
	 */
public:
	BlockType block_type;

	// static const constexpr float BLOCK_RENDER_SIZE = 1.0f / 16.0f;

	cgp::vec3 getColor();

	Block();
};