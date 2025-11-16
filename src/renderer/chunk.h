#pragma once

#include <vector>
#include "raylib.h"
#include "textures.h"

namespace Renderer
{
    constexpr int CHUNK_SIZE = 16;
    constexpr int CHUNK_HEIGHT = 32;

    class Chunk {
    public:
        // data
        void add_block(Vector3 local_pos, BLOCK block);
        // model
        void build_model();
        void draw_model(Vector3 position) const;

    private:
        // data
        std::array<std::array<std::array<BLOCK, CHUNK_SIZE>, CHUNK_HEIGHT>, CHUNK_SIZE> blocks{};
        bool is_block_surrounded(Vector3 local_pos) const;
        // model
        Model model{};
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
        void add_block_to_model(Vector3 local_pos, BLOCK block);
    };
}