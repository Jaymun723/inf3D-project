#pragma once
#include "../environment.hpp"
#include "int3.hpp"
#include "chunk.hpp"
#include <unordered_map>

class ChunkManager
{
public:
  ChunkManager();

  void Update(vec3 player_position);

  int AddChunk(vec3 position);
  Chunk &GetChunk(int id);
  Chunk &GetChunk(int id, Directions neigh);
  Chunk &GetChunk(Int3 position);

  void Render(const environment_structure &environment);
  void WireRender(const environment_structure &environment);

private:
  std::vector<Chunk> m_chunks;
  std::unordered_map<Int3, Chunk *, Int3Hasher> m_chunk_map;
  Int3 m_previous_player_chunk_position;

  std::vector<Int3> m_loaded_chunks;

  // std::vector<Chunk *> m_render_list;
  // std::vector<Chunk *> m_load_list;
  // std::vector<Chunk *> m_unload_list;

  // void UpdateLoadList();
  // void UpdateUnloadList();
  // void UpdateRenderList();
};