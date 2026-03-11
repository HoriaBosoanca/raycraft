#pragma once

#include "world.h"
#include "btBulletDynamicsCommon.h"
#include "meshgen.h"
#include "textures.h"

namespace World
{
    struct Chunk {
        ChunkData chunk_data;
        MeshGen::ChunkModel chunk_model = {};
        btRigidBody* chunk_rb = nullptr;
    };

    constexpr int32_t WORLD_SIZE = 6;
    extern std::map<ChunkPos, Chunk> world;

    void generate();
    BLOCK get_block(WorldPos world_pos);
    void set_block(WorldPos world_pos, BLOCK block);
    void render();
    void build_chunk(ChunkPos chunk_pos);
    void build_chunks();
    bool is_block_surrounded(WorldPos world_pos);
}
