#include "chunk_model.h"
#include <climits>
#include "chunk.h"
#include "raylib.h"
#include "raymath.h"
#include "textures.h"
#include "world.h"

namespace ChunkModel
{
	void ChunkModel::add_block_to_mesh(const World::LocalPos local_pos, const World::BLOCK block) {
		if (!World::is_opaque(block)) {
			for (int i = 0; i < cube.triangleCount * 3; i++) {
				indices3.push_back(cube.indices[i] + vertexCount3);
			}
			for (int i = 0; i < cube.vertexCount * 3; i += 3) {
				vertices3.push_back(cube.vertices[i + 0] + static_cast<float>(local_pos.x));
				vertices3.push_back(cube.vertices[i + 1] + static_cast<float>(local_pos.y));
				vertices3.push_back(cube.vertices[i + 2] + static_cast<float>(local_pos.z));
			}
			normals3.insert(normals3.end(), cube.normals, cube.normals + cube.vertexCount * 3);
			for (int i = 0; i < 48; i++) {
				if (i%2 == 0) { // even = U
					// using "1.0f-" to flip the texture
					texcoords3.push_back((1.0f-cube.texcoords[i] + t_map[block][i/8].x) * ATLAS_UNIT);
				} else { // odd = V
					texcoords3.push_back((1.0f-cube.texcoords[i] + t_map[block][i/8].y) * ATLAS_UNIT);
				}
			}
			vertexCount3 += cube.vertexCount;
			triangleCount3 += cube.triangleCount;
		} else if (vertexCount1+cube.vertexCount <= USHRT_MAX) {
            for (int i = 0; i < cube.triangleCount * 3; i++) {
                indices1.push_back(cube.indices[i] + vertexCount1);
            }
            for (int i = 0; i < cube.vertexCount * 3; i += 3) {
                vertices1.push_back(cube.vertices[i + 0] + static_cast<float>(local_pos.x));
                vertices1.push_back(cube.vertices[i + 1] + static_cast<float>(local_pos.y));
                vertices1.push_back(cube.vertices[i + 2] + static_cast<float>(local_pos.z));
            }
            normals1.insert(normals1.end(), cube.normals, cube.normals + cube.vertexCount * 3);
            for (int i = 0; i < 48; i++) {
                if (i%2 == 0) { // even = U
                    // using "1.0f-" to flip the texture
                    texcoords1.push_back((1.0f-cube.texcoords[i] + t_map[block][i/8].x) * ATLAS_UNIT);
                } else { // odd = V
                    texcoords1.push_back((1.0f-cube.texcoords[i] + t_map[block][i/8].y) * ATLAS_UNIT);
                }
            }
            vertexCount1 += cube.vertexCount;
            triangleCount1 += cube.triangleCount;
        } else {
            for (int i = 0; i < cube.triangleCount * 3; i++) {
                indices2.push_back(cube.indices[i] + vertexCount2);
            }
            for (int i = 0; i < cube.vertexCount * 3; i += 3) {
                vertices2.push_back(cube.vertices[i + 0] + static_cast<float>(local_pos.x));
                vertices2.push_back(cube.vertices[i + 1] + static_cast<float>(local_pos.y));
                vertices2.push_back(cube.vertices[i + 2] + static_cast<float>(local_pos.z));
            }
            normals2.insert(normals2.end(), cube.normals, cube.normals + cube.vertexCount * 3);
            for (int i = 0; i < 48; i++) {
                if (i%2 == 0) { // even = U
                    // using "1.0f-" to flip the texture
                    texcoords2.push_back((1.0f-cube.texcoords[i] + t_map[block][i/8].x) * ATLAS_UNIT);
                } else { // odd = V
                    texcoords2.push_back((1.0f-cube.texcoords[i] + t_map[block][i/8].y) * ATLAS_UNIT);
                }
            }
            vertexCount2 += cube.vertexCount;
            triangleCount2 += cube.triangleCount;
        }
	}

	void ChunkModel::rebuild_model(const World::ChunkData& chunk_data, const World::ChunkPos chunk_pos) {
		if (model.meshes == nullptr) {
			model.transform = MatrixIdentity();
			model.meshCount = 3;
			model.meshes = new Mesh[3];
			model.materialCount = 1;
			model.materials = new Material[1];
			model.meshMaterial = new int[3];
			model.materials[0] = default_mat;
			model.meshMaterial[0] = 0;
			model.meshMaterial[1] = 0;
			model.meshMaterial[2] = 0;
			model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ATLAS;
		} else {
			for (int i = 0; i < model.meshCount; i++)
				if (model.meshes[i].vertexCount > 0)
					UnloadMesh(model.meshes[i]);
		}

		Mesh &mesh1 = model.meshes[0] = {},
			 &mesh2 = model.meshes[1] = {},
			 &mesh3 = model.meshes[2] = {};

		for (uint32_t x = 0; x < World::CHUNK_SIZE; x++) {
			for (uint32_t y = 0; y < World::CHUNK_HEIGHT; y++) {
				for (uint32_t z = 0; z < World::CHUNK_SIZE; z++) {
					if (const World::LocalPos local_pos = {x, y, z};
						chunk_data.get_block(local_pos) != World::BLOCK::AIR && !World::is_block_surrounded(chunk_pos + World::LocalPos{x, y, z})) {
						add_block_to_mesh({x, y, z}, chunk_data.get_block(local_pos));
					}
				}
			}
		}

		mesh1.vertexCount = vertexCount1;
		vertexCount1 = 0;
		mesh1.triangleCount = triangleCount1;
		triangleCount1 = 0;
		mesh1.vertices = static_cast<float*>(RL_MALLOC(vertices1.size() * sizeof(float)));
		std::copy(vertices1.begin(), vertices1.end(), mesh1.vertices);
		vertices1.clear();
		vertices1.shrink_to_fit();
		mesh1.normals = static_cast<float*>(RL_MALLOC(normals1.size() * sizeof(float)));
		std::copy(normals1.begin(), normals1.end(), mesh1.normals);
		normals1.clear();
		normals1.shrink_to_fit();
		mesh1.texcoords = static_cast<float*>(RL_MALLOC(texcoords1.size() * sizeof(float)));
		std::copy(texcoords1.begin(), texcoords1.end(), mesh1.texcoords);
		texcoords1.clear();
		texcoords1.shrink_to_fit();
		mesh1.indices = static_cast<uint16_t*>(RL_MALLOC(indices1.size() * sizeof(uint16_t)));
		std::copy(indices1.begin(), indices1.end(), mesh1.indices);
		indices1.clear();
		indices1.shrink_to_fit();
		UploadMesh(&mesh1, false);
		if (vertexCount2 > 0) {
			mesh2.vertexCount = vertexCount2;
			vertexCount2 = 0;
			mesh2.triangleCount = triangleCount2;
			triangleCount2 = 0;
			mesh2.vertices = static_cast<float*>(RL_MALLOC(vertices2.size() * sizeof(float)));
			std::copy(vertices2.begin(), vertices2.end(), mesh2.vertices);
			vertices2.clear();
			vertices2.shrink_to_fit();
			mesh2.normals = static_cast<float*>(RL_MALLOC(normals2.size() * sizeof(float)));
			std::copy(normals2.begin(), normals2.end(), mesh2.normals);
			normals2.clear();
			normals2.shrink_to_fit();
			mesh2.texcoords = static_cast<float*>(RL_MALLOC(texcoords2.size() * sizeof(float)));
			std::copy(texcoords2.begin(), texcoords2.end(), mesh2.texcoords);
			texcoords2.clear();
			texcoords2.shrink_to_fit();
			mesh2.indices = static_cast<uint16_t*>(RL_MALLOC(indices2.size() * sizeof(uint16_t)));
			std::copy(indices2.begin(), indices2.end(), mesh2.indices);
			indices2.clear();
			indices2.shrink_to_fit();
			UploadMesh(&mesh2, false);
		}
		if (vertexCount3 > 0) {
			mesh3.vertexCount = vertexCount3;
			vertexCount3 = 0;
			mesh3.triangleCount = triangleCount3;
			triangleCount3 = 0;
			mesh3.vertices = static_cast<float*>(RL_MALLOC(vertices3.size() * sizeof(float)));
			std::copy(vertices3.begin(), vertices3.end(), mesh3.vertices);
			vertices3.clear();
			vertices3.shrink_to_fit();
			mesh3.normals = static_cast<float*>(RL_MALLOC(normals3.size() * sizeof(float)));
			std::copy(normals3.begin(), normals3.end(), mesh3.normals);
			normals3.clear();
			normals3.shrink_to_fit();
			mesh3.texcoords = static_cast<float*>(RL_MALLOC(texcoords3.size() * sizeof(float)));
			std::copy(texcoords3.begin(), texcoords3.end(), mesh3.texcoords);
			texcoords3.clear();
			texcoords3.shrink_to_fit();
			mesh3.indices = static_cast<uint16_t*>(RL_MALLOC(indices3.size() * sizeof(uint16_t)));
			std::copy(indices3.begin(), indices3.end(), mesh3.indices);
			indices3.clear();
			indices3.shrink_to_fit();
			UploadMesh(&mesh3, false);
		}
	}

	void ChunkModel::render(const World::ChunkPos chunk_pos) const {
		const World::WorldPos pos = chunk_pos + World::LocalPos{0, 0, 0};
		DrawModel(model, {static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z)}, 1.0f, WHITE);
	}
}
