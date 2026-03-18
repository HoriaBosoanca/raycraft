#include "chunk.h"
#include <cmath>

namespace World
{
    ChunkPos ChunkPos::operator+(const ChunkPos& other) const {
        return {x + other.x, z + other.z};
    }
    WorldPos ChunkPos::operator+(const LocalPos& other) const {
        return {
            x * CHUNK_SIZE + static_cast<int32_t>(other.x),
            static_cast<int32_t>(other.y),
            z * CHUNK_SIZE + static_cast<int32_t>(other.z)};
    }
    int32_t ChunkPos::operator-(const ChunkPos& other) const {
        return std::abs(other.x - x) + std::abs(other.z - z);
    }
    bool ChunkPos::operator<(const ChunkPos& other) const {
        if (x != other.x) return x < other.x;
        return z < other.z;
    }
    bool ChunkPos::operator==(const ChunkPos& other) const {
        return x == other.x && z == other.z;
    }
    WorldPos WorldPos::operator+(const WorldPos& other) const {
        return {x + static_cast<int32_t>(other.x), y + other.y, z + static_cast<int32_t>(other.z)};
    }
    uint32_t floor_mod(const int32_t n) {
        const int32_t mod = n % CHUNK_SIZE;
        return mod < 0 ? mod + CHUNK_SIZE : mod;
    }
    LocalPos WorldPos::get_local_pos() const {
        return LocalPos{floor_mod(x), static_cast<uint32_t>(y), floor_mod(z)};
    }
    int32_t floor_div(const int32_t n) {
        return n >= 0 ? n / CHUNK_SIZE : (n-CHUNK_SIZE+1) / CHUNK_SIZE;
    }
    ChunkPos WorldPos::get_chunk_pos() const {
        return ChunkPos{floor_div(x), floor_div(z)};
    }
    bool WorldPos::operator<(const WorldPos& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }

    void ChunkData::set_block(const LocalPos local_pos, const BLOCK block) {
        if (local_pos.x < U_CHUNK_SIZE && local_pos.y < U_CHUNK_HEIGHT && local_pos.z < U_CHUNK_SIZE)
            blocks[local_pos.x][local_pos.y][local_pos.z] = block;
    }
    BLOCK ChunkData::get_block(const LocalPos local_pos) const {
        return blocks[local_pos.x][local_pos.y][local_pos.z];
    }
}
