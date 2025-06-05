#include "chunk_manager.hpp"
#include <functional>
#include <iostream>

std::size_t Int3Hasher::operator()(const Int3 &k) const
{
  std::size_t h1 = std::hash<int>()(k.x);
  std::size_t h2 = std::hash<int>()(k.y);
  std::size_t h3 = std::hash<int>()(k.z);
  // Combine hashes (this is a basic technique, better ones exist)
  return h1 ^ (h2 << 1) ^ (h3 << 2);
}

ChunkManager::ChunkManager()
{
  // std::cout << "Hello from ChunkManager constructor !" << std::endl;
}

void ChunkManager::Update(vec3 player_position)
{
  // Step
}

int ChunkManager::AddChunk(vec3 position)
{
  // Construct Chunk directly in the vector
  m_chunks.emplace_back(position);

  Chunk &chunk = m_chunks.back(); // Reference to the chunk just added

  chunk.FullChunk();
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
  Int3 position = Int3(origin.m_position).neighboor(neigh);

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

Int3::Int3(int x, int y, int z) : x(x), y(y), z(z) {}

Int3::Int3(vec3 position) : x(position.x), y(position.y), z(position.z) {}

Int3 Int3::neighboor(Directions dir)
{
  switch (dir)
  {
  case Up:
    return Int3(x, y, z + 1);
  case Down:
    return Int3(x, y, z - 1);
  case Front:
    return Int3(x + 1, y, z);
  case Back:
    return Int3(x - 1, y, z);
  case Left:
    return Int3(x, y + 1, z);
  case Right:
    return Int3(x, y - 1, z);
  }
}

bool Int3::operator==(const Int3 &other) const
{
  return x == other.x && y == other.y && z == other.z;
}