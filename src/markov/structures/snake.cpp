#include "snake.hpp"

bool AppearOnHeadRule::applies_to(const Chunk &C, const vec3 &pos) const
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    if (C.m_pBlocks[x][y][z].block_type == BlockType_Head)
    {
        vec3 directions[6] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0),
                              vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};
        for (const vec3 &dir : directions)
        {
            vec3 new_pos = pos + dir;
            if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x || new_pos.y < 0 ||
                new_pos.y >= C.BLOCK_CHUNK_SIZE.y || new_pos.z < 0 ||
                new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
            {
                continue; // Out of bounds
            }
            if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z]
                    .block_type == BlockType_Empty)
            {
                return true; // Found an empty block adjacent to the head
            }
        }
    }
    return false;
}

void AppearOnHeadRule::apply(Chunk &C, const vec3 &pos) const
{
    // std::cout << "Applying AppearOnHeadRule at position: " << pos << std::endl;
    int MAX_TRIES = 30;
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    int random_idx = rand() % 6; // Randomly choose one of the 6 directions
    for (int i = 0; i < MAX_TRIES; ++i)
    {
        vec3 directions[6] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0),
                              vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};
        vec3 new_pos = pos + directions[random_idx];
        if (new_pos.x < 0 || new_pos.x >= C.BLOCK_CHUNK_SIZE.x 
            || new_pos.y < 0 ||new_pos.y >= C.BLOCK_CHUNK_SIZE.y || 
            new_pos.z < 0 || new_pos.z >= C.BLOCK_CHUNK_SIZE.z)
        {
            continue;
        }
        if (C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type == BlockType_Empty)
        {
            C.m_pBlocks[x][y][z].block_type =
                BlockType_Rainbow; // Set the current block to Default
            C.m_pBlocks[(int)new_pos.x][(int)new_pos.y][(int)new_pos.z].block_type = BlockType_Head;
            break;
        }
    }
}

ExtendedMR AppearOnHead = ExtendedMR({std::make_shared<AppearOnHeadRule>()});

int build_snake_aux(Chunk &C, int step)
{
    int x;
    int y;
    int z;
    switch (step)
    {
    case 0:
        x = C.BLOCK_CHUNK_SIZE.x / 4 + rand() % (C.BLOCK_CHUNK_SIZE.x / 2);
		y = C.BLOCK_CHUNK_SIZE.y / 4 + rand() % (C.BLOCK_CHUNK_SIZE.y / 2);
		z = C.BLOCK_CHUNK_SIZE.z / 2 + rand() % (C.BLOCK_CHUNK_SIZE.z / 2);
        C.m_pBlocks[x][y][z].block_type = BlockType_Head;
        step = 1;
        break;

    case 1:
        if (AppearOnHead.applyRule(C, 1))
        {
            break;
        }
        else
        {
            step = -2;
            break;
        }

    case -2:
        return -1;

    case -1:
        return -1;

    default:
        std::cout << "Invalid step in build_snake_aux: " << step << std::endl;
        break;

    }

    return step;
}

int build_snake(Chunk &C, int step, int speed)
{
    for (int i = 0; i < speed; ++i)
    {
        step = build_snake_aux(C, step);
        if (step < 0) {
            break;
        }
    }
    return step;
}