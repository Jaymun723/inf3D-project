#include "chunk_manager.hpp"
#include <functional>

std::size_t Int3Hasher::operator()(const Int3 &k) const {
  std::size_t h1 = std::hash<int>()(k.x);
  std::size_t h2 = std::hash<int>()(k.y);
  std::size_t h3 = std::hash<int>()(k.z);
  // Combine hashes (this is a basic technique, better ones exist)
  return h1 ^ (h2 << 1) ^ (h3 << 2);
}

ChunkManager::ChunkManager() {
  // std::cout << "Hello from ChunkManager constructor !" << std::endl;
}

int ChunkManager::AddChunk(vec3 position) {
  Chunk chunk = Chunk();

  chunk.FullChunk();
  chunk.Load();

  // chunk.m_position = position;

  m_chunks.push_back(chunk);
  // m_chunk_map[Int3(position)] = &chunk;

  return m_chunks.size() - 1;
}

// void ChunkManager::Update(environment_structure environment) {
//   UpdateLoadList();
//   UpdateUnloadList();
// }

void ChunkManager::Render(environment_structure environment) {
  for (Chunk &chunk : m_chunks) {
    chunk.Render(environment);
  }
}

void ChunkManager::WireRender(environment_structure environment) {
  for (Chunk &chunk : m_chunks) {
    chunk.WireRender(environment);
  }
}

// void ChunkManager::UpdateLoadList() {
//   int lNumOfChunksLoaded = 0;

//   for (Chunk *chunk : m_load_list) {
//     if (!chunk->IsLoaded()) {

//       if (lNumOfChunksLoaded != MAX_CHUNK) {

//         chunk->Load(); // Increase the chunks loaded count

//         lNumOfChunksLoaded++;

//         // m_forceVisibilityUpdate = true;
//       }
//     }

//   } // Clear the load list (every frame)

//   m_load_list.clear();
// }

// void ChunkManager::UpdateUnloadList() {
//   for (int i = 0; i < m_unload_list.size(); i++) {
//     delete m_load_list[i];
//   }
//   m_unload_list.clear();
// }

// void ChunkManager::UpdateRenderList() {
//   m_render_list.clear();
//   for (Chunk *chunk : m_load_list) {
//     if (/* visibility test && */ true) {
//       if (chunk->m_should_render) {
//         chunk->m_should_render = false;

//         m_render_list.push_back(chunk);
//       }
//     } else {
//       m_unload_list.push_back(chunk);
//     }
//   }
// }

Int3::Int3(int x, int y, int z) : x(x), y(y), z(z) {}

Int3::Int3(vec3 position) : x(position.x), y(position.y), z(position.z) {}

bool Int3::operator==(const Int3 &other) const {
  return x == other.x && y == other.y && z == other.z;
}