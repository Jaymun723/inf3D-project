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
  Block();
  void setActive(bool to_active);
  BlockType block_type;
};