#pragma once

#include "chunk.h"

namespace Renderer
{
    extern std::map<ChunkPos, Chunk> world;
    extern const int32_t WORLD_SIZE;
    void render_world();
    void build_world_models();
    void add_chunk_to_world(const Chunk& chunk, ChunkPos chunk_pos);
    BLOCK get_block(WorldPos world_pos);
    bool is_block_surrounded(WorldPos world_pos);
}