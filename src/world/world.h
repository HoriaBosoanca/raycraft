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
    extern bool STEP_PHYSICS;
    extern btDynamicsWorld* dynamics_world;
    void setup();
    void update();

    void build_chunk(ChunkPos chunk_pos);
    BLOCK get_block(WorldPos world_pos);
    void set_block(WorldPos world_pos, BLOCK block);
    bool is_block_surrounded(WorldPos world_pos);
}
