#pragma once

#include "../environment.hpp"
#include "block.hpp"
#include "cgp/cgp.hpp"

class Chunk {
public:
  Chunk();
  ~Chunk();

  void Render(environment_structure environment);
  void WireRender(environment_structure environment);
  void FullChunk();
  void HalfChunk();
  void RandomChunk(float density);

  bool IsLoaded();
  void Load();
  void UnLoad();

  void UpdateMesh();

  bool m_should_render;

  cgp::vec3 m_position;

  static const int CHUNK_SIZE = 16;
  Block ***m_pBlocks;

private: // The blocks data
  void CreateMesh();
  bool m_loaded;

  int m_active_blocks;

  // cgp::mesh chunk_mesh;
  cgp::mesh_drawable m_chunk_drawable_mesh;
};