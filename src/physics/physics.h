#pragma once

#include "btBulletDynamicsCommon.h"
#include "world.h"

namespace Physics
{
    extern bool STEP_PHYSICS;
    extern btDynamicsWorld* dynamics_world;
    void init();
    void update();
    btVector3 get_player_pos();
    btVector3 get_player_velocity();
    void set_player_velocity(btVector3 vel);
    bool is_player_grounded();
    void player_set_target_block(World::BLOCK block, btVector3 from, btVector3 to, float reach_dist);
}
