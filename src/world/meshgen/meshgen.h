#pragma once

#include "textures.h"
#include <vector>

namespace MeshGen
{
	class ChunkModel {
	public:
		Model model = {};
		int vertexCount1 = 0;
		int vertexCount2 = 0;
		int triangleCount1 = 0;
		int triangleCount2 = 0;
		std::vector<float> vertices1;
		std::vector<float> vertices2;
		std::vector<unsigned short> indices1;
		std::vector<unsigned short> indices2;
		std::vector<float> normals1;
		std::vector<float> normals2;
		std::vector<float> texcoords1;
		std::vector<float> texcoords2;
		void rebuild_model(const World::ChunkData& chunk_data, World::ChunkPos chunk_pos);
		void render(World::ChunkPos chunk_pos) const;
	private:
		void add_block_to_mesh(World::LocalPos local_pos, World::BLOCK block);
	};
}
