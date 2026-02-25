#pragma once

#include <unordered_map>
#include "chunk.h"

namespace Renderer
{
    struct ChunkPosHash {
        size_t operator()(const ChunkPos& v) const noexcept {
            return std::hash<int>{}(v.x) ^ (std::hash<int>{}(v.z) << 1);
        }
    };
    struct ChunkPosEqual {
        bool operator()(const ChunkPos& a, const ChunkPos& b) const noexcept {
            return a.x == b.x && a.z == b.z;
        }
    };
    extern std::unordered_map<ChunkPos, Chunk, ChunkPosHash, ChunkPosEqual> world;
    extern const int32_t WORLD_SIZE;
    void render_world();
    void build_world_models();
    void add_chunk_to_world(const Chunk& chunk, ChunkPos chunk_pos);
    BLOCK get_block(Vector2 chunk_pos, Vector3 local_pos);
    bool is_block_surrounded(WorldPos world_pos);
}