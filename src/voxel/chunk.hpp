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
  Chunk(Int3 position);
  ~Chunk();

  void Render(const environment_structure &environment);
  void WireRender(const environment_structure &environment);
  void FullChunk();
  void HalfChunk();
  void FlatChunk();
  void RandomChunk(float density);

  bool IsLoaded();
  void Load();
  void UnLoad();

  void UpdateMesh();

  bool m_should_render = true;

  cgp::vec3 m_position;
  int step = 0;
  int speed = 1;
  int build_type = -1;
  bool BuildFunction();

  // How many blocks in each direction
  static const Int3 BLOCK_CHUNK_SIZE;
  // How many units to render the chunk in each direction
  static const Int3 RENDER_CHUNK_SIZE;

  Block ***m_pBlocks;

private: // The blocks data
  void CreateMesh();
  void AddQuadToMesh(cgp::mesh &chunk_mesh, const cgp::vec3 &block_position, Directions face_direction, const cgp::vec3 &color);
  cgp::vec3 CalculateNormal(Directions face_direction);

  bool m_loaded = false;
  int m_active_blocks = 0;
  cgp::mesh_drawable m_chunk_drawable_mesh;
};