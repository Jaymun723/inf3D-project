#include "chunk.hpp"
#include <iostream>

using namespace cgp;

const Int3 Chunk::BLOCK_CHUNK_SIZE = Int3(32, 32, 32);

const Int3 Chunk::RENDER_CHUNK_SIZE = Int3(2, 2, 2);

Chunk::Chunk()
{ // Create the blocks
  m_loaded = false;
  m_should_render = false;

  m_pBlocks = new Block **[BLOCK_CHUNK_SIZE.x];
  for (int i = 0; i < BLOCK_CHUNK_SIZE.x; i++)
  {
    m_pBlocks[i] = new Block *[BLOCK_CHUNK_SIZE.y];
    for (int j = 0; j < BLOCK_CHUNK_SIZE.y; j++)
    {
      m_pBlocks[i][j] = new Block[BLOCK_CHUNK_SIZE.z];
    }
  }
}

Chunk::Chunk(vec3 position) : Chunk()
{
  m_position = position;
}

void Chunk::Render(const environment_structure &environment)
{
  m_chunk_drawable_mesh.model.translation = m_position;
  m_chunk_drawable_mesh.model.scaling_xyz = {((float)RENDER_CHUNK_SIZE.x) / ((float)BLOCK_CHUNK_SIZE.x),
                                             ((float)RENDER_CHUNK_SIZE.y) / ((float)BLOCK_CHUNK_SIZE.y),
                                             ((float)RENDER_CHUNK_SIZE.z) / ((float)BLOCK_CHUNK_SIZE.z)};
  draw(m_chunk_drawable_mesh, environment);
}

void Chunk::WireRender(const environment_structure &environment)
{
  m_chunk_drawable_mesh.model.translation = m_position;
  draw_wireframe(m_chunk_drawable_mesh, environment);
}

Chunk::~Chunk()
{ // Delete the blocks
  std::cout << "Deconstructor called on Chunk" << std::endl;
  m_chunk_drawable_mesh.clear();

  for (int i = 0; i < BLOCK_CHUNK_SIZE.x; ++i)
  {
    for (int j = 0; j < BLOCK_CHUNK_SIZE.y; ++j)
    {
      delete[] m_pBlocks[i][j];
    }
    delete[] m_pBlocks[i];
  }
  delete[] m_pBlocks;
}

void Chunk::CreateMesh()
{
  // std::cout << "CreateMesh() called" << std::endl;
  mesh chunk_mesh;
  m_active_blocks = 0;

  for (int x = 0; x < BLOCK_CHUNK_SIZE.x; x++)
  {
    for (int y = 0; y < BLOCK_CHUNK_SIZE.y; y++)
    {
      for (int z = 0; z < BLOCK_CHUNK_SIZE.z; z++)
      {
        Block pBlock = m_pBlocks[x][y][z];
        if (pBlock.block_type != BlockType_Empty)
        {
          m_active_blocks++;
          vec3 offset = vec3(0.5, 0.5, 0.5);

          vec3 center = offset + vec3(x, y, z);

          mesh cube = mesh_primitive_cube(center,
                                          1);

          // std::cout << "Cube with " << center << std::endl;

          for (vec3 &color : cube.color)
          {
            color = pBlock.getColor();
          }

          chunk_mesh.push_back(cube);
        }
      }
    }
  }

  if (m_active_blocks > 0)
  {
    m_chunk_drawable_mesh.initialize_data_on_gpu(chunk_mesh);
  }
}

void Chunk::FullChunk()
{
  for (int x = 0; x < BLOCK_CHUNK_SIZE.x; x++)
  {
    for (int y = 0; y < BLOCK_CHUNK_SIZE.y; y++)
    {
      for (int z = 0; z < BLOCK_CHUNK_SIZE.z; z++)
      {
        m_pBlocks[x][y][z].block_type = BlockType_Default;
      }
    }
  }
}

void Chunk::HalfChunk()
{
  for (int x = 0; x < BLOCK_CHUNK_SIZE.x; x++)
  {
    for (int y = 0; y < BLOCK_CHUNK_SIZE.y; y++)
    {
      for (int z = 0; z < (x + y) / 2; z++)
      {
        m_pBlocks[x][y][z].block_type = BlockType_Default;
      }
    }
  }
}

void Chunk::RandomChunk(float density)
{
  for (int x = 0; x < BLOCK_CHUNK_SIZE.x; x++)
  {
    for (int y = 0; y < BLOCK_CHUNK_SIZE.y; y++)
    {
      for (int z = 0; z < BLOCK_CHUNK_SIZE.z; z++)
      {
        if (static_cast<float>(rand()) / RAND_MAX < density)
        {
          m_pBlocks[x][y][z].block_type = BlockType_Default;
        }
      }
    }
  }
}

bool Chunk::IsLoaded() { return m_loaded; }

void Chunk::Load()
{
  if (!m_loaded)
  {
    CreateMesh();
    // m_loaded = true;
  }
}

void Chunk::UpdateMesh()
{
  m_loaded = true;
  m_chunk_drawable_mesh.clear();
  CreateMesh();
}

void Chunk::UnLoad()
{
  if (m_loaded)
  {
    m_loaded = false;
  }
}
