#pragma once

#include <cstdint>

namespace World
{
    constexpr uint32_t U_CHUNK_SIZE = 16;
    constexpr uint32_t U_CHUNK_HEIGHT = 32;
    constexpr int32_t CHUNK_SIZE = 16;
    constexpr int32_t CHUNK_HEIGHT = 48;

    struct LocalPos; struct ChunkPos; struct WorldPos;
    struct LocalPos {
        uint32_t x, y, z;
        LocalPos(const uint32_t _x, const uint32_t _y, const uint32_t _z) {
            x = _x % U_CHUNK_SIZE;
            y = _y % U_CHUNK_HEIGHT;
            z = _z % U_CHUNK_SIZE;
        }
    };
    struct ChunkPos {
        int32_t x, z;
        ChunkPos operator+(const ChunkPos& other) const;
        WorldPos operator+(const LocalPos& other) const;
        bool operator<(const ChunkPos& other) const;
    };
    struct WorldPos {
        int32_t x, y, z;
        WorldPos(const int32_t x, const int32_t y, const int32_t z) : x(x), y(y), z(z) {}
        WorldPos operator+(const WorldPos& other) const;
        LocalPos get_local_pos() const;
        ChunkPos get_chunk_pos() const;
        bool operator<(const WorldPos& other) const;
    };

    enum class BLOCK : uint8_t {
        AIR,
        GRASS,
        DIRT,
        STONE,
        OAK_PLANKS,
        OAK_LOG,
        OAK_LEAVES
    };

    class ChunkData {
    public:
        void set_block(LocalPos local_pos, BLOCK block);
        BLOCK get_block(LocalPos local_pos) const;
    private:
        BLOCK blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]{};
    };
}