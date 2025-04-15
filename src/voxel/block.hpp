#pragma once

enum BlockType {
	BlockType_Default
};

class Block {
  /**
   * @brief Défini un block dans notre monde.
   */
public:
  bool active; /** @property Doit-on afficher le block ou non ? */ // Pourquoi pas une fonction, et le paramètre en privé ?
  BlockType block_type;

  Block();
  void setActive(bool active);
  void setBlockType(BlockType block_type);
};