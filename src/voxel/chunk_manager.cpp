#include "chunk_manager.hpp"

ChunkManager::ChunkManager() {}

void ChunkManager::Update(environment_structure environment) {
  UpdateLoadList();
  UpdateUnloadList();
}

void ChunkManager::Render(environment_structure environment) {}

void ChunkManager::UpdateLoadList() {
  int lNumOfChunksLoaded = 0;

  for (Chunk *chunk : m_load_list) {
    if (!chunk->IsLoaded()) {

      if (lNumOfChunksLoaded != MAX_CHUNK) {

        chunk->Load(); // Increase the chunks loaded count

        lNumOfChunksLoaded++;

        // m_forceVisibilityUpdate = true;
      }
    }

  } // Clear the load list (every frame)

  m_load_list.clear();
}

void ChunkManager::UpdateUnloadList() {
  for (int i = 0; i < m_unload_list.size(); i++) {
    delete m_load_list[i];
  }
  m_unload_list.clear();
}

void ChunkManager::UpdateRenderList() {
  m_render_list.clear();
  for (Chunk *chunk : m_load_list) {
    if (/* visibility test && */ true) {
      if (chunk->m_should_render) {
        chunk->m_should_render = false;

        m_render_list.push_back(chunk);
      }
    } else {
      m_unload_list.push_back(chunk);
    }
  }
}
