#pragma once

#include "world.h"
#include "btBulletDynamicsCommon.h"
#include "chunk_model.h"
#include "textures.h"

namespace World
{
    struct Chunk {
        ChunkData chunk_data;
        ChunkModel::ChunkModel chunk_model = {};
        btRigidBody* chunk_rb = nullptr;
    };

    constexpr int32_t WORLD_SIZE = 6;
    extern std::map<ChunkPos, Chunk> world;
    extern bool STEP_PHYSICS;
    extern btDynamicsWorld* dynamics_world;
    void setup();
    void update();

    void update_chunk_around_block(WorldPos world_pos);
    BLOCK get_block(WorldPos world_pos);
    void set_block(WorldPos world_pos, BLOCK block);
    bool is_opaque(BLOCK block);
    bool is_block_surrounded(WorldPos world_pos);
}
