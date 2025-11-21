#pragma once

#include <unordered_map>
#include "chunk.h"

namespace Renderer
{
    struct Vector2Hash {
        size_t operator()(const Vector2& v) const noexcept {
            return std::hash<float>{}(v.x) ^ (std::hash<float>{}(v.y) << 1);
        }
    };
    struct Vector2Equal {
        bool operator()(const Vector2& a, const Vector2& b) const noexcept {
            return a.x == b.x && a.y == b.y;
        }
    };
    extern std::unordered_map<Vector2, Chunk, Vector2Hash, Vector2Equal> world;
    extern const int WORLD_SIZE;
    void render_world();
    void build_world_models();
    void set_chunk(const Chunk& chunk, Vector2 chunk_pos);
    BLOCK get_block(Vector2 chunk_pos, Vector3 local_pos);
    bool is_block_surrounded(Vector2 chunk_pos, Vector3 local_pos);
}