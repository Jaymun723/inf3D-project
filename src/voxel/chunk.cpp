#include "chunk.hpp"
#include "markov/structures/road.hpp"
#include "markov/structures/river.hpp"
#include <iostream>

using namespace cgp;

const Int3 Chunk::BLOCK_CHUNK_SIZE = Int3(32, 32, 32);

const Int3 Chunk::RENDER_CHUNK_SIZE = Int3(1, 1, 1);

Chunk::Chunk()
{
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

Chunk::Chunk(cgp::vec3 position) : Chunk()
{
  m_position = position;
}

Chunk::Chunk(Int3 position) : Chunk(position.ToVec())
{
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
  m_chunk_drawable_mesh.model.scaling_xyz = {((float)RENDER_CHUNK_SIZE.x) / ((float)BLOCK_CHUNK_SIZE.x),
                                             ((float)RENDER_CHUNK_SIZE.y) / ((float)BLOCK_CHUNK_SIZE.y),
                                             ((float)RENDER_CHUNK_SIZE.z) / ((float)BLOCK_CHUNK_SIZE.z)};
  draw_wireframe(m_chunk_drawable_mesh, environment);
}

Chunk::~Chunk()
{ // Delete the blocks
  std::cout << "Deconstructor called on Chunk at (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")" << std::endl;
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
        Block &current_block = m_pBlocks[x][y][z];
        if (current_block.block_type != BlockType_Empty)
        {
          m_active_blocks++;
          vec3 block_position = vec3(x, y, z);
          vec3 block_color = current_block.getColor();

          // Check each face and only add if it's visible (not occluded by adjacent block)

          // Front face (+X direction)
          if (x == BLOCK_CHUNK_SIZE.x - 1 || m_pBlocks[x + 1][y][z].block_type == BlockType_Empty)
          {
            AddQuadToMesh(chunk_mesh, block_position, Front, block_color);
          }

          // Back face (-X direction)
          if (x == 0 || m_pBlocks[x - 1][y][z].block_type == BlockType_Empty)
          {
            AddQuadToMesh(chunk_mesh, block_position, Back, block_color);
          }

          // Right face (+Y direction)
          if (y == BLOCK_CHUNK_SIZE.y - 1 || m_pBlocks[x][y + 1][z].block_type == BlockType_Empty)
          {
            AddQuadToMesh(chunk_mesh, block_position, Right, block_color);
          }

          // Left face (-Y direction)
          if (y == 0 || m_pBlocks[x][y - 1][z].block_type == BlockType_Empty)
          {
            AddQuadToMesh(chunk_mesh, block_position, Left, block_color);
          }

          // Top face (+Z direction)
          if (z == BLOCK_CHUNK_SIZE.z - 1 || m_pBlocks[x][y][z + 1].block_type == BlockType_Empty)
          {
            AddQuadToMesh(chunk_mesh, block_position, Up, block_color);
          }

          // Bottom face (-Z direction)
          if (z == 0 || m_pBlocks[x][y][z - 1].block_type == BlockType_Empty)
          {
            AddQuadToMesh(chunk_mesh, block_position, Down, block_color);
          }
        }
      }
    }
  }

  if (m_active_blocks > 0)
  {
    chunk_mesh.fill_empty_field();
    m_chunk_drawable_mesh.initialize_data_on_gpu(chunk_mesh);
  }
}

// Helper method to add a single quad face to the mesh
void Chunk::AddQuadToMesh(mesh &chunk_mesh, const vec3 &block_position, Directions face_direction, const vec3 &color)
{
  vec3 center = block_position + vec3(0.5f, 0.5f, 0.5f);

  // Define the 4 vertices of the quad based on face direction
  std::vector<vec3> vertices(4);

  switch (face_direction)
  {
  case Front:                                        // +X face
    vertices[0] = center + vec3(0.5f, -0.5f, -0.5f); // bottom-left
    vertices[1] = center + vec3(0.5f, 0.5f, -0.5f);  // bottom-right
    vertices[2] = center + vec3(0.5f, 0.5f, 0.5f);   // top-right
    vertices[3] = center + vec3(0.5f, -0.5f, 0.5f);  // top-left
    break;

  case Back:                                          // -X face
    vertices[0] = center + vec3(-0.5f, 0.5f, -0.5f);  // bottom-left
    vertices[1] = center + vec3(-0.5f, -0.5f, -0.5f); // bottom-right
    vertices[2] = center + vec3(-0.5f, -0.5f, 0.5f);  // top-right
    vertices[3] = center + vec3(-0.5f, 0.5f, 0.5f);   // top-left
    break;

  case Right:                                        // +Y face
    vertices[0] = center + vec3(0.5f, 0.5f, -0.5f);  // bottom-left
    vertices[1] = center + vec3(-0.5f, 0.5f, -0.5f); // bottom-right
    vertices[2] = center + vec3(-0.5f, 0.5f, 0.5f);  // top-right
    vertices[3] = center + vec3(0.5f, 0.5f, 0.5f);   // top-left
    break;

  case Left:                                          // -Y face
    vertices[0] = center + vec3(-0.5f, -0.5f, -0.5f); // bottom-left
    vertices[1] = center + vec3(0.5f, -0.5f, -0.5f);  // bottom-right
    vertices[2] = center + vec3(0.5f, -0.5f, 0.5f);   // top-right
    vertices[3] = center + vec3(-0.5f, -0.5f, 0.5f);  // top-left
    break;

  case Up:                                           // +Z face
    vertices[0] = center + vec3(-0.5f, -0.5f, 0.5f); // bottom-left
    vertices[1] = center + vec3(0.5f, -0.5f, 0.5f);  // bottom-right
    vertices[2] = center + vec3(0.5f, 0.5f, 0.5f);   // top-right
    vertices[3] = center + vec3(-0.5f, 0.5f, 0.5f);  // top-left
    break;

  case Down:                                          // -Z face
    vertices[0] = center + vec3(-0.5f, 0.5f, -0.5f);  // bottom-left
    vertices[1] = center + vec3(0.5f, 0.5f, -0.5f);   // bottom-right
    vertices[2] = center + vec3(0.5f, -0.5f, -0.5f);  // top-right
    vertices[3] = center + vec3(-0.5f, -0.5f, -0.5f); // top-left
    break;
  }

  // Calculate normal for the face
  vec3 normal = CalculateNormal(face_direction);

  // Add the quad as two triangles (0-1-2 and 0-2-3)
  size_t base_index = chunk_mesh.position.size();

  // Add vertices
  for (const vec3 &vertex : vertices)
  {
    chunk_mesh.position.push_back(vertex);
    chunk_mesh.color.push_back(color);
    chunk_mesh.normal.push_back(normal);
  }

  // Add triangle indices for the quad (two triangles)
  // Triangle 1: 0-1-2
  chunk_mesh.connectivity.push_back(uint3(base_index, base_index + 1, base_index + 2));
  // Triangle 2: 0-2-3
  chunk_mesh.connectivity.push_back(uint3(base_index, base_index + 2, base_index + 3));
}

// Helper method to calculate face normal
vec3 Chunk::CalculateNormal(Directions face_direction)
{
  switch (face_direction)
  {
  case Front:
    return vec3(1.0f, 0.0f, 0.0f);
  case Back:
    return vec3(-1.0f, 0.0f, 0.0f);
  case Right:
    return vec3(0.0f, 1.0f, 0.0f);
  case Left:
    return vec3(0.0f, -1.0f, 0.0f);
  case Up:
    return vec3(0.0f, 0.0f, 1.0f);
  case Down:
    return vec3(0.0f, 0.0f, -1.0f);
  default:
    return vec3(0.0f, 0.0f, 1.0f);
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

void Chunk::FlatChunk()
{
  for (int x = 0; x < BLOCK_CHUNK_SIZE.x; x++)
  {
    for (int y = 0; y < BLOCK_CHUNK_SIZE.y; y++)
    {
      m_pBlocks[x][y][0].block_type = BlockType_Default;
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
    m_should_render = false;
    m_loaded = true;
  }
}

void Chunk::UpdateMesh()
{
  if (!m_should_render)
  {
    return;
  }

  if (m_loaded)
  {
    m_chunk_drawable_mesh.clear();
  }

  m_should_render = false;
  m_loaded = true;
  CreateMesh();
}

void Chunk::UnLoad()
{
  if (m_loaded)
  {
    m_chunk_drawable_mesh.clear();
    m_should_render = true;
    m_loaded = false;
  }
}


void Chunk::BuildFunction()
{
	step =  build_river(*this, step, speed);
}
