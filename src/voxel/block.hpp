#pragma once

enum BlockType { BlockType_Empty, BlockType_Default };

class Block {
  /**
   * @brief Défini un block dans notre monde.
   */
public:
  BlockType block_type;

  static const constexpr float BLOCK_RENDER_SIZE = 1.0f / 16.0f;

  Block();
};