#include "chunk.hpp"

using namespace cgp;

Chunk::Chunk() { // Create the blocks
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

Chunk::~Chunk() { // Delete the blocks
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
        if (m_pBlocks[x][y][z].block_type != BlockType_Empty) {
          m_active_blocks++;
          vec3 offset = Block::BLOCK_RENDER_SIZE * vec3(0.5, 0.5, 0.5);
          chunk_mesh.push_back(mesh_primitive_cube(
              offset + Block::BLOCK_RENDER_SIZE * vec3(x, y, z),
              Block::BLOCK_RENDER_SIZE));
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

Block Chunk::GetBlock(int x, int y, int z) { return m_pBlocks[x][y][z]; }