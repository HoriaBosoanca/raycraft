#pragma once

#include <vector>
#include "raylib.h"
#include "textures.h"

namespace Renderer
{
    constexpr uint32_t U_CHUNK_SIZE = 16;
    constexpr uint32_t U_CHUNK_HEIGHT = 32;
    constexpr int32_t CHUNK_SIZE = 16;
    constexpr int32_t CHUNK_HEIGHT = 32;

    struct LocalPos; struct ChunkPos; struct WorldPos;
    struct LocalPos {
        uint32_t x, y, z;
        LocalPos(const uint32_t _x, const uint32_t _y, const uint32_t _z) {
            x = _x % U_CHUNK_SIZE;
            y = _y % U_CHUNK_HEIGHT;
            z = _z % U_CHUNK_SIZE;
        }
        // LocalPos operator+(const LocalPos& other) const;
    };
    struct ChunkPos {
        int32_t x, z;
        ChunkPos operator+(const ChunkPos& other) const;
        WorldPos operator+(const LocalPos& other) const;
    };
    struct WorldPos {
        int32_t x, y, z;
        WorldPos(const int32_t x, const int32_t y, const int32_t z) : x(x), y(y), z(z) {}
        WorldPos operator+(const WorldPos& other) const;
        LocalPos get_local_pos() const;
        ChunkPos get_chunk_pos() const;
    };

    class Chunk {
    public:
        // data
        void set_block(LocalPos local_pos, BLOCK block);
        BLOCK get_block(LocalPos local_pos) const;
        // model
        void build_model(ChunkPos chunk_pos);
        void draw_model(WorldPos world_pos) const;

    private:
        // data
        BLOCK blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]{};
        bool is_block_surrounded(LocalPos local_pos) const;
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
        void add_block_to_model(LocalPos local_pos, BLOCK block);
    };
}
