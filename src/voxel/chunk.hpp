#include "../environment.hpp"
#include "block.hpp"
#include "cgp/cgp.hpp"

class Chunk {
public:
  Chunk();
  ~Chunk();
  void Render(environment_structure environment);

  void CreateMesh();

  cgp::vec3 m_position;

  static const int CHUNK_SIZE = 16;

  void FullChunk();

  Block ***m_pBlocks;

private: // The blocks data
  int m_active_blocks;

  // cgp::mesh chunk_mesh;
  cgp::mesh_drawable m_chunk_drawable_mesh;
};