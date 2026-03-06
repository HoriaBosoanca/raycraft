#pragma once

#include "btBulletDynamicsCommon.h"
#include "chunk.h"

namespace Physics
{
    extern bool STEP_PHYSICS;
    void setup();
    void update();
    void add_block(Renderer::WorldPos world_pos);
    btVector3 get_player_pos();
    btVector3 get_player_velocity();
    void set_player_velocity(btVector3 vel);
    bool is_player_grounded();
}
