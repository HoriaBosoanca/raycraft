#pragma once

#include "textures.h"
#include <vector>

namespace ChunkModel
{
	class ChunkModel {
	public:
		void rebuild_model(const World::ChunkData& chunk_data, World::ChunkPos chunk_pos);
		void render(World::ChunkPos chunk_pos) const;
	private:
		Model model = {};
		int vertexCount1 = 0;
		int vertexCount2 = 0;
		int vertexCount3 = 0;
		int triangleCount1 = 0;
		int triangleCount2 = 0;
		int triangleCount3 = 0;
		std::vector<float> vertices1;
		std::vector<float> vertices2;
		std::vector<float> vertices3;
		std::vector<float> normals1;
		std::vector<float> normals2;
		std::vector<float> normals3;
		std::vector<float> texcoords1;
		std::vector<float> texcoords2;
		std::vector<float> texcoords3;
		std::vector<uint16_t> indices1;
		std::vector<uint16_t> indices2;
		std::vector<uint16_t> indices3;
		void add_block_to_mesh(World::LocalPos local_pos, World::BLOCK block);
	};
}
