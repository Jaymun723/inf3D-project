#pragma once
#include "../environment.hpp"
#include "chunk.hpp"
#include <unordered_map>

enum Directions
{
  Up,    //  0,  0,  1
  Down,  //  0,  0, -1
  Front, //  1,  0,  0
  Back,  // -1,  0,  0
  Left,  //  0,  1,  0
  Right  //  0, -1,  0
};

struct Int3
{
  int x;
  int y;
  int z;

  Int3(int x, int y, int z);
  Int3(vec3 position);

  bool operator==(const Int3 &other) const;
};

struct Int3Hasher
{
  std::size_t operator()(const Int3 &k) const;
};

class ChunkManager
{
public:
  ChunkManager();

  // void Update(vec3environment_structure environment);

  int AddChunk(vec3 position);
  // Chunk &GetChunk(int id);
  // Chunk &GetChunk(int id, Directions neigh);
  // Chunk &GetChunk(vec3 position);

  void Render(const environment_structure &environment);
  void WireRender(const environment_structure &environment);

private:
  std::vector<Chunk> m_chunks;
  std::unordered_map<Int3, Chunk *, Int3Hasher> m_chunk_map;

  // std::vector<Chunk *> m_render_list;
  // std::vector<Chunk *> m_load_list;
  // std::vector<Chunk *> m_unload_list;

  // void UpdateLoadList();
  // void UpdateUnloadList();
  // void UpdateRenderList();
};