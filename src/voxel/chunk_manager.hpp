#pragma once
#include "../environment.hpp"
#include "chunk.hpp"

class ChunkManager {
public:
  ChunkManager();

  void Update(environment_structure environment);
  void Render(environment_structure environment);

  static const int MAX_CHUNK = 10;

private:
  std::vector<Chunk *> m_render_list;
  std::vector<Chunk *> m_load_list;
  std::vector<Chunk *> m_unload_list;

  void UpdateLoadList();
  void UpdateUnloadList();
  void UpdateRenderList();
};