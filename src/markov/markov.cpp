#include "markov.hpp"
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>
// #include "voxel/chunk.hpp"

BasicMR::BasicMR() { rules = {}; }

BasicMR::BasicMR(const std::vector<BasicR> &initialRules)
    : rules(std::move(initialRules)) {}

void BasicMR::addRule(const BasicR &rule) { rules.push_back(rule); }

std::vector<int> generate_random_indices(int size, int count)
{
  std::vector<int> indices(size);
  for (int i = 0; i < size; ++i)
  {
    indices[i] = i;
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(indices.begin(), indices.end(), g);
  indices.resize(count);
  return indices;
}

void BasicMR::applyRule(Chunk &C, int limit)
{
  bool found = true;
  int i = 0;
  while (found && (i < limit || limit < 0))
  {
    found = false;
    for (const auto &rule : rules)
    {
      for (int x = 0; x < C.BLOCK_CHUNK_SIZE.x; ++x)
      {
        std::vector<int> rx = generate_random_indices(C.BLOCK_CHUNK_SIZE.x, C.BLOCK_CHUNK_SIZE.x);
        for (int y = 0; y < C.BLOCK_CHUNK_SIZE.y; ++y)
        {
          std::vector<int> ry = generate_random_indices(C.BLOCK_CHUNK_SIZE.y, C.BLOCK_CHUNK_SIZE.y);
          for (int z = 0; z < C.BLOCK_CHUNK_SIZE.z; ++z)
          {
            std::vector<int> rz = generate_random_indices(C.BLOCK_CHUNK_SIZE.z, C.BLOCK_CHUNK_SIZE.z);
            vec3 pos = {(int)rx[x], (int)ry[y], (int)rz[z]};
            std::cout << pos << std::endl;
            if (i < limit || limit < 0)
            {
              if (rule.applies_to(C, pos))
              {
                found = true;
                rule.apply(C, pos);
                ++i;
              }
            }
          }
        }
      }
    }
  }
  C.Load();
}

void BasicMR::applyRule(Chunk &C) { applyRule(C, -1); }

bool ExtendedMR::applyRule(Chunk &C, int limit)
{
  bool found = true;
  int i = 0;
  std::vector<std::shared_ptr<Rule>> rules_to_apply;
  std::vector<vec3> positions_to_apply;

  found = false;
  for (const auto &rule : rules)
  {
    std::vector<int> positions = generate_random_indices(C.BLOCK_CHUNK_SIZE.x * C.BLOCK_CHUNK_SIZE.y * C.BLOCK_CHUNK_SIZE.z, C.BLOCK_CHUNK_SIZE.x * C.BLOCK_CHUNK_SIZE.y * C.BLOCK_CHUNK_SIZE.z);
    for (int pos_index = 0; pos_index < positions.size(); ++pos_index)
    {
      int z = positions[pos_index] / (C.BLOCK_CHUNK_SIZE.z * C.BLOCK_CHUNK_SIZE.z);
      int y = (positions[pos_index] / C.BLOCK_CHUNK_SIZE.y) % C.BLOCK_CHUNK_SIZE.y;
      int x = positions[pos_index] % C.BLOCK_CHUNK_SIZE.x;
      vec3 pos = {x, y, z};
      if (i < limit || limit < 0)
      {
        if (rule->applies_to(C, pos))
        {
          found = true;
          rules_to_apply.push_back(rule);
          positions_to_apply.push_back(pos);
          ++i;
        }
      }
      else
      {
        break;
      }
    }
  }
  for (size_t j = 0; j < rules_to_apply.size(); ++j)
  {
    rules_to_apply[j]->apply(C, positions_to_apply[j]);
  }
  return (i > 0);
}

bool ExtendedMR::applyRule(Chunk &C) { return applyRule(C, -1); }
