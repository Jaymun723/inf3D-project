#include "chunk_manager.hpp"
#include <functional>
#include <iostream>
#include <algorithm>

const Int3 ChunkManager::RENDER_DISTANCE = Int3(4, 0, 4);

ChunkManager::ChunkManager()
{
  // std::cout << "Hello from ChunkManager constructor !" << std::endl;
  m_previous_player_chunk_position = Int3(-10, -10, -10);
}

bool ChunkManager::ChunkExists(const Int3 &pos) const
{
  return m_chunk_map.find(pos) != m_chunk_map.end();
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
  for (auto it = m_loaded_chunks.begin(); it != m_loaded_chunks.end();)
  {
    // if (it->Manhattan(player_chunk_position) > RENDER_DISTANCE)
    if (std::abs(it->x - player_chunk_position.x) > RENDER_DISTANCE.x || std::abs(it->y - player_chunk_position.y) > RENDER_DISTANCE.y ||
        std::abs(it->z - player_chunk_position.z) > RENDER_DISTANCE.z)
    {
      Chunk &chunk = GetChunk(*it);
      chunk.UnLoad();
      it = m_loaded_chunks.erase(it); // Erase returns next valid iterator
    }
    else
    {
      ++it;
    }
  }

  // DON'T clear m_loaded_chunks here!
  // m_loaded_chunks.clear(); // <-- Remove this line

  // Step 3, verify that chunks around the player are all present (if not create them) and loaded (if not load them)
  for (int dx = -RENDER_DISTANCE.x; dx <= RENDER_DISTANCE.x; dx++)
  {
    for (int dy = -RENDER_DISTANCE.y; dy <= RENDER_DISTANCE.y; dy++)
    {
      for (int dz = -RENDER_DISTANCE.z; dz <= RENDER_DISTANCE.z; dz++)
      {
        // std::cout << " dz = " << dz << std::endl;
        Int3 p = player_chunk_position + Int3(dx, dy, dz);

        // Only add to loaded_chunks if it's not already there
        if (std::find(m_loaded_chunks.begin(), m_loaded_chunks.end(), p) == m_loaded_chunks.end())
        {
          m_loaded_chunks.push_back(p);
        }

        // std::cout << "p.x = " << p.x << " p.y = " << p.y << " p.z = " << p.z << std::endl;
        if (!ChunkExists(p))
        {
          AddChunk(p);
        }
        // Only call UpdateMesh if the chunk exists and is not already loaded
        Chunk *chunk = m_chunk_map[p];
        chunk->UpdateMesh();
      }
    }
  }
}

int ChunkManager::AddChunk(vec3 position)
{
  return AddChunk(Int3(position));
}

int ChunkManager::AddChunk(Int3 position)
{
  m_chunks.emplace_back(position); // Constructs in-place
  Chunk &chunk = m_chunks.back();  // Reference is valid

  // chunk.HalfChunk(); // Initialize contents
  // chunk.FullChunk();
  chunk.FlatChunk();

  m_chunk_map[position] = &chunk; // Store pointer

  // Don't add to loaded_chunks here - let Update() handle it
  // m_loaded_chunks.push_back(position); // <-- Remove this line

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
