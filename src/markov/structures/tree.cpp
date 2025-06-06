#include "tree.hpp"

bool ExtendTrunkRule::applies_to(const Chunk &C, const vec3 &pos) const
{
    if (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type != BlockType_Empty)
    {
        return false;
    }
    if (pos.z > 0 && C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z - 1].block_type == BlockType_Wood_Tmp)
    {
        return true;
    }
    return false;
}

void ExtendTrunkRule::apply(Chunk &C, const vec3 &pos) const
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    int MAX_TRUNK_HEIGHT = 10;
    if (rand() % 7 == 0 || z >= MAX_TRUNK_HEIGHT)
    {
        C.m_pBlocks[x][y][z].block_type = BlockType_Leaf_Tmp;
    }
    else
    {
        C.m_pBlocks[x][y][z].block_type = BlockType_Wood_Tmp;
    }
    C.m_pBlocks[x][y][z - 1].block_type = BlockType_Wood;
}

ExtendedMR ExtendTrunk = ExtendedMR({std::make_shared<ExtendTrunkRule>()});

bool ExtendLeafRule::applies_to(const Chunk &C, const vec3 &pos) const
{
    return (C.m_pBlocks[(int)pos.x][(int)pos.y][(int)pos.z].block_type == BlockType_Leaf_Tmp);
}

void ExtendLeafRule::apply(Chunk &C, const vec3 &pos) const
{
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    C.m_pBlocks[x][y][z].block_type = BlockType_Leaf;

    int count_new_tmp = 0;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dz = -1; dz <= 1; ++dz)
            {
                if (dx == 0 && dy == 0 && dz == 0)
                    continue; // Skip the current block
                int nx = x + dx;
                int ny = y + dy;
                int nz = z + dz;
                if (nx >= 0 && nx < C.BLOCK_CHUNK_SIZE.x && ny >= 0 && ny < C.BLOCK_CHUNK_SIZE.y && nz >= 0 && nz < C.BLOCK_CHUNK_SIZE.z && C.m_pBlocks[nx][ny][nz].block_type == BlockType_Empty)
                {
                    if (rand() % 4 > 0)
                    {
                        if (rand() % 10 == 0 && count_new_tmp < 2)
                        {
                            count_new_tmp++;
                            C.m_pBlocks[nx][ny][nz].block_type = BlockType_Leaf_Tmp;
                        }
                        else
                        {
                            C.m_pBlocks[nx][ny][nz].block_type = BlockType_Leaf;
                        }
                    }
                }
            }
        }
    }
}

ExtendedMR ExtendLeaf = ExtendedMR({std::make_shared<ExtendLeafRule>()});

int build_tree_aux(Chunk &C, int step)
{
    int x, y;
    int x_t, y_t, z_t;
    switch (step)
    {
    case 0:
		x = rand() % (C.BLOCK_CHUNK_SIZE.x / 2) + C.BLOCK_CHUNK_SIZE.x / 4;
		y = rand() % (C.BLOCK_CHUNK_SIZE.y / 2) + C.BLOCK_CHUNK_SIZE.y / 4;
        C.m_pBlocks[x][y][0].block_type = BlockType_Grass;
        step = 1;
        break;

    case 1:
        if (BuildGroundGrass.applyRule(C, 10))
        {
            break;
        }
        step = 2;
        break;

    case 2:
        x_t = rand() % (C.BLOCK_CHUNK_SIZE.x / 2) + C.BLOCK_CHUNK_SIZE.x / 4;
        y_t = rand() % (C.BLOCK_CHUNK_SIZE.y / 2) + C.BLOCK_CHUNK_SIZE.y / 4;
        z_t = 1;
        C.m_pBlocks[x_t][y_t][z_t].block_type = BlockType_Wood_Tmp;
        step = 3;
        break;

    case 3:
        if (ExtendTrunk.applyRule(C, 1))
        {
            break;
        }
        step = 4;
        break;

    case 4:
        if (ExtendLeaf.applyRule(C, 1))
        {
            break;
        }
        step = -2;
        break;

    case -2:
        return -1;

    case -1:
        return -1;

    default:
        std::cout << "Invalid step in build_tree_aux: " << step << std::endl;
        break;
    }
    return step;
}

int build_tree(Chunk &C, int step, int speed)
{
    for (int i = 0; i < speed; ++i)
    {
        step = build_tree_aux(C, step);
        if (step < 0) {
            break;
        }
    }
    return step;
}