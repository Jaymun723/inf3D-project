#include "chunk.hpp"

using namespace cgp;

Chunk::Chunk() { // Create the blocks
  m_loaded = false;
  m_should_render = false;

  m_pBlocks = new Block **[CHUNK_SIZE];
  for (int i = 0; i < CHUNK_SIZE; i++) {
    m_pBlocks[i] = new Block *[CHUNK_SIZE];
    for (int j = 0; j < CHUNK_SIZE; j++) {
      m_pBlocks[i][j] = new Block[CHUNK_SIZE];
    }
  }
}

void Chunk::Render(environment_structure environment) {
  m_chunk_drawable_mesh.model.translation = m_position;
  draw(m_chunk_drawable_mesh, environment);
}

void Chunk::WireRender(environment_structure environment) {
  m_chunk_drawable_mesh.model.translation = m_position;
  draw_wireframe(m_chunk_drawable_mesh, environment);
}

Chunk::~Chunk() { // Delete the blocks
  m_chunk_drawable_mesh.clear();

  for (int i = 0; i < CHUNK_SIZE; ++i) {
    for (int j = 0; j < CHUNK_SIZE; ++j) {
      delete[] m_pBlocks[i][j];
    }
    delete[] m_pBlocks[i];
  }
  delete[] m_pBlocks;
}

void Chunk::CreateMesh() {
  mesh chunk_mesh;
  m_active_blocks = 0;

  for (int x = 0; x < CHUNK_SIZE; x++) {
    for (int y = 0; y < CHUNK_SIZE; y++) {
      for (int z = 0; z < CHUNK_SIZE; z++) {
        Block pBlock = m_pBlocks[x][y][z];
        if (pBlock.block_type != BlockType_Empty) {
          m_active_blocks++;
          vec3 offset = Block::BLOCK_RENDER_SIZE * vec3(0.5, 0.5, 0.5);

          mesh cube = mesh_primitive_cube(offset + Block::BLOCK_RENDER_SIZE *
                                                       vec3(x, y, z),
                                          Block::BLOCK_RENDER_SIZE);

          for (vec3 &color : cube.color) {
            color = pBlock.getColor();
          }

          chunk_mesh.push_back(cube);
        }
      }
    }
  }

  if (m_active_blocks > 0) {
    m_chunk_drawable_mesh.initialize_data_on_gpu(chunk_mesh);
  }
}

void Chunk::FullChunk() {
  for (int x = 0; x < CHUNK_SIZE; x++) {
    for (int y = 0; y < CHUNK_SIZE; y++) {
      for (int z = 0; z < CHUNK_SIZE; z++) {
        m_pBlocks[x][y][z].block_type = BlockType_Default;
      }
    }
  }
}

void Chunk::HalfChunk() {
  for (int x = 0; x < CHUNK_SIZE; x++) {
    for (int y = 0; y < CHUNK_SIZE; y++) {
      for (int z = 0; z < (x + y) / 2; z++) {
        m_pBlocks[x][y][z].block_type = BlockType_Default;
      }
    }
  }
}

bool Chunk::IsLoaded() { return m_loaded; }

void Chunk::Load() {
  if (!m_loaded) {
    CreateMesh();
    // m_loaded = true;
  }
}

void Chunk::UpdateMesh() {
  m_loaded = true;
  m_chunk_drawable_mesh.clear();
  CreateMesh();
}

void Chunk::UnLoad() {
  if (m_loaded) {
    m_loaded = false;
  }
}
