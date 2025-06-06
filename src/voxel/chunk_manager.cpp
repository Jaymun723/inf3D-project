#include "chunk_manager.hpp"
#include <functional>
#include <iostream>
#include <algorithm>

const Int3 ChunkManager::RENDER_DISTANCE = Int3(12, 12, 0);
const Int3 ChunkManager::COMPUTE_DISTANCE = Int3(1, 1, 0);

ChunkManager::ChunkManager()
{
  m_previous_player_chunk_position = Int3(-10, -10, -10);
}

bool ChunkManager::ChunkExists(const Int3 &pos) const
{
  return m_chunk_map.find(pos) != m_chunk_map.end();
}

void ChunkManager::Update(vec3 player_position, int frame_count)
{
  Int3 player_chunk_position = Int3(player_position);

  // Step 1 updates nearest chunks.
  for (int dx = -COMPUTE_DISTANCE.x; dx <= COMPUTE_DISTANCE.x; dx++)
  {
    for (int dy = -COMPUTE_DISTANCE.y; dy <= COMPUTE_DISTANCE.y; dy++)
    {
      Int3 p = player_chunk_position + Int3(dx, dy, 0);
      if (ChunkExists(p))
      {
        Chunk *chunk = m_chunk_map[p];
        bool still_computing = chunk->BuildFunction();
        chunk->m_should_render = still_computing;
        if (still_computing)
        {
          chunk->UpdateMesh();
        }
      }
    }
  }

  // Step 2, compute chunk player position

  // If same position as before, do nothing
  if (player_chunk_position == m_previous_player_chunk_position)
  {
    return;
  }
  m_previous_player_chunk_position = player_chunk_position;

  // Step 3, iterate over loaded chunks, unload the distant ones
  for (auto it = m_loaded_chunks.begin(); it != m_loaded_chunks.end();)
  {
    // Is the chunk distant ?
    if (std::abs(it->x - player_chunk_position.x) > RENDER_DISTANCE.x || std::abs(it->y - player_chunk_position.y) > RENDER_DISTANCE.y ||
        std::abs(it->z - player_chunk_position.z) > RENDER_DISTANCE.z)
    {
      Int3 pos = *it;
      Chunk &chunk = GetChunk(pos);
      chunk.UnLoad();
      it = m_loaded_chunks.erase(it); // Erase returns next valid iterator
    }
    else
    {
      ++it;
    }
  }

  // Step 4, verify that chunks around the player are all present (if not create them) and loaded (if not load them)
  for (int dx = -RENDER_DISTANCE.x; dx <= RENDER_DISTANCE.x; dx++)
  {
    for (int dy = -RENDER_DISTANCE.y; dy <= RENDER_DISTANCE.y; dy++)
    {
      for (int dz = 0; dz <= 0; dz++)
      {
        Int3 p = player_chunk_position + Int3(dx, dy, dz);

        // Only add to loaded_chunks if it's not already there
        if (std::find(m_loaded_chunks.begin(), m_loaded_chunks.end(), p) == m_loaded_chunks.end())
        {
          m_loaded_chunks.push_back(p);
        }

        if (!ChunkExists(p))
        {
          int id = AddChunk(p);
          if (p.x % 2 == 0 || p.y % 2 == 0)
          {
            m_chunks[id].BuildFunction(); // For roads
          }
        }

        Chunk *chunk = m_chunk_map[p];
        chunk->Load();
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

  m_chunk_map[position] = &chunk; // Store pointer

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

void ChunkManager::Render(const environment_structure &environment)
{
  for (Chunk &chunk : m_chunks)
  {
    chunk.Render(environment);
  }
}

void ChunkManager::WireRender(const environment_structure &environment)
{
  GetChunk(m_previous_player_chunk_position).WireRender(environment);
}