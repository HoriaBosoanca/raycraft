#include "chunk.h"
#include <iostream>
#include <raymath.h>
#include "textures.h"

namespace Renderer
{
    void Chunk::add_block(Vector3 local_pos, BLOCK block) {
        blocks[(int)local_pos.x][(int)local_pos.y][(int)local_pos.z] = block;
    }

    bool Chunk::is_block_surrounded(Vector3 local_pos) const {
        if (local_pos.x == 0 || local_pos.x == CHUNK_SIZE-1 || local_pos.y == 0 || local_pos.y == CHUNK_HEIGHT-1 || local_pos.z == 0 || local_pos.z == CHUNK_SIZE-1) return false;
        if (blocks[(int)local_pos.x-1][(int)local_pos.y][(int)local_pos.z] != BLOCK::AIR &&
            blocks[(int)local_pos.x+1][(int)local_pos.y][(int)local_pos.z] != BLOCK::AIR &&
            blocks[(int)local_pos.x][(int)local_pos.y-1][(int)local_pos.z] != BLOCK::AIR &&
            blocks[(int)local_pos.x][(int)local_pos.y+1][(int)local_pos.z] != BLOCK::AIR &&
            blocks[(int)local_pos.x][(int)local_pos.y][(int)local_pos.z-1] != BLOCK::AIR &&
            blocks[(int)local_pos.x][(int)local_pos.y][(int)local_pos.z+1] != BLOCK::AIR) return true;
        return false;
    }

    void Chunk::add_block_to_model(Vector3 local_pos, BLOCK block) {
        if (vertexCount1+cube.vertexCount <= USHRT_MAX) {
            for (int i = 0; i < cube.triangleCount * 3; i++) {
                indices1.push_back(cube.indices[i] + vertexCount1);
            }
            for (int i = 0; i < cube.vertexCount * 3; i += 3) {
                vertices1.push_back(cube.vertices[i + 0] + local_pos.x);
                vertices1.push_back(cube.vertices[i + 1] + local_pos.y);
                vertices1.push_back(cube.vertices[i + 2] + local_pos.z);
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
                vertices2.push_back(cube.vertices[i + 0] + local_pos.x);
                vertices2.push_back(cube.vertices[i + 1] + local_pos.y);
                vertices2.push_back(cube.vertices[i + 2] + local_pos.z);
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

    void Chunk::build_model() {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    if (blocks[x][y][z] != BLOCK::AIR && !is_block_surrounded(Vector3{(float) x, (float) y, (float) z})) {
                        add_block_to_model(Vector3{(float) x, (float) y, (float) z}, blocks[x][y][z]);
                    }
                }
            }
        }

        Mesh mesh1 = { 0 };
        mesh1.vertexCount = vertexCount1;
        mesh1.triangleCount = triangleCount1;
        mesh1.vertices = vertices1.data();
        mesh1.normals = normals1.data();
        mesh1.texcoords = texcoords1.data();
        mesh1.indices = indices1.data();
        UploadMesh(&mesh1, false);

        Mesh mesh2 = { 0 };
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

        model = { 0 };
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

    void Chunk::draw_model(Vector3 position) const {
        DrawModel(model, position, 1.0, WHITE);
    }
}