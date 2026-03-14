#include "meshgen.h"
#include <climits>
#include "chunk.h"
#include "raylib.h"
#include "raymath.h"
#include "textures.h"
#include "world.h"

namespace MeshGen
{
	void ChunkModel::add_block_to_mesh(const World::LocalPos local_pos, const World::BLOCK block) {
		if (vertexCount1+cube.vertexCount <= USHRT_MAX) {
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

	void ChunkModel::build_model(const World::ChunkData& chunk_data, const World::ChunkPos chunk_pos) {
		Mesh mesh1 = {}, mesh2 = {};

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
		mesh1.triangleCount = triangleCount1;
		mesh1.vertices = vertices1.data();
		mesh1.normals = normals1.data();
		mesh1.texcoords = texcoords1.data();
		mesh1.indices = indices1.data();
		UploadMesh(&mesh1, false);

		mesh2.vertexCount = vertexCount2;
		mesh2.triangleCount = triangleCount2;
		mesh2.vertices = vertices2.data();
		mesh2.normals = normals2.data();
		mesh2.texcoords = texcoords2.data();
		mesh2.indices = indices2.data();
		UploadMesh(&mesh2, false);

		vertexCount1 = 0;
		vertexCount2 = 0;
		triangleCount1 = 0;
		triangleCount2 = 0;
		vertices1.clear();
		vertices2.clear();
		normals1.clear();
		normals2.clear();
		texcoords1.clear();
		texcoords2.clear();
		indices1.clear();
		indices2.clear();

		delete[] model.meshes;
		delete[] model.materials;
		delete[] model.meshMaterial;

		model = {};
		model.transform = MatrixIdentity();
		model.meshCount = 2;
		model.meshes = new Mesh[2];
		model.meshes[0] = mesh1;
		model.meshes[1] = mesh2;
		model.materialCount = 1;
		model.materials = new Material[1];
		model.materials[0] = default_mat;
		model.meshMaterial = new int[2];
		model.meshMaterial[0] = 0;
		model.meshMaterial[1] = 0;
		model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ATLAS;
	}

	void ChunkModel::render(const World::ChunkPos chunk_pos) const {
		const World::WorldPos pos = chunk_pos + World::LocalPos{0, 0, 0};
		DrawModel(model, {static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z)}, 1.0f, WHITE);
	}
}
