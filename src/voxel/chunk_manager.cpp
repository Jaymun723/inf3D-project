#include "chunk_manager.hpp"
#include <functional>
#include <iostream>

ChunkManager::ChunkManager()
{
  // std::cout << "Hello from ChunkManager constructor !" << std::endl;
}

void ChunkManager::Update(vec3 player_position)
{
  // Step 1, compute chunk player position
  Int3 player_chunk_position = Int3(player_position / Chunk::RENDER_CHUNK_SIZE.ToVec());
  // If same position as before, do nothing
  if (player_chunk_position == m_previous_player_chunk_position)
  {
    return;
  }
  m_previous_player_chunk_position = player_chunk_position;

  // Step 2, iterate over loaded chunks, unload the distant ones

  // Step 3, verify that chunks around the player are all present, if not create them, and loaded, if not load them
}

int ChunkManager::AddChunk(vec3 position)
{
  // Construct Chunk directly in the vector
  m_chunks.emplace_back(position);

  Chunk &chunk = m_chunks.back(); // Reference to the chunk just added

  // chunk.FullChunk();
  chunk.HalfChunk();
  chunk.Load();

  m_chunk_map[Int3(position)] = &chunk;

  return m_chunks.size() - 1;
}

Chunk &ChunkManager::GetChunk(int id)
{
  return m_chunks.at(id);
}

Chunk &ChunkManager::GetChunk(int id, Directions neigh)
{
  Chunk &origin = m_chunks.at(id);
  Int3 position = Int3(origin.m_position).Neighboor(neigh);

  return GetChunk(position);
}

Chunk &ChunkManager::GetChunk(Int3 position)
{
  auto it = m_chunk_map.find(position);
  if (it == m_chunk_map.end())
  {
    throw std::out_of_range("Chunk not found at given position.");
  }

  return *(it->second); // Dereference the pointer to get the reference
}

// void ChunkManager::Update(environment_structure environment) {
//   UpdateLoadList();
//   UpdateUnloadList();
// }

void ChunkManager::Render(const environment_structure &environment)
{
  for (Chunk &chunk : m_chunks)
  {
    // std::cout << "Rendering chunk from manager" << std::endl;
    // std::cout << chunk.m_should_render << std::endl;
    // std::cout << chunk.m_active_blocks << std::endl;
    chunk.Render(environment);
  }
}

void ChunkManager::WireRender(const environment_structure &environment)
{
  for (Chunk &chunk : m_chunks)
  {
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
