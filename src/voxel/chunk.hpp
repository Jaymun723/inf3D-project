#pragma once

#include "../environment.hpp"
#include "block.hpp"
#include "cgp/cgp.hpp"
#include "int3.hpp"

class Chunk
{
public:
  Chunk();
  Chunk(vec3 position);
  ~Chunk();

  void Render(const environment_structure &environment);
  void WireRender(const environment_structure &environment);
  void FullChunk();
  void HalfChunk();
  void RandomChunk(float density);

  bool IsLoaded();
  void Load();
  void UnLoad();

  void UpdateMesh();

  bool m_should_render;

  cgp::vec3 m_position;

  // How many blocks in each direction
  static const Int3 BLOCK_CHUNK_SIZE;
  // How many units to render the chunk in each direction
  static const Int3 RENDER_CHUNK_SIZE;

  Block ***m_pBlocks;

private: // The blocks data
  void CreateMesh();
  bool m_loaded;

  int m_active_blocks;

  // cgp::mesh chunk_mesh;
  cgp::mesh_drawable m_chunk_drawable_mesh;
};