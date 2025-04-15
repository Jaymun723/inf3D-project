#include "block.hpp"

class Chunk {
	public:
	Chunk();
	~Chunk();
	//void Update(float dt);
	// void Render(OpenGLRenderer* pRenderer);
	static const int CHUNK_SIZE = 16;
	private: // The blocks data
		Block*** m_pBlocks;
};