#pragma once

#include "raylib.h"
#include "btBulletDynamicsCommon.h"
#include "chunk.h"

namespace Player
{
    extern Camera camera;
    void setup();
    void update();
    void setup_player_rb();
    btVector3 get_physics_player_pos();
    btVector3 get_player_velocity();
    void set_player_velocity(btVector3 vel);
    bool is_player_grounded();
    void player_set_target_block(World::BLOCK block, btVector3 from, btVector3 to, float reach_dist, bool replace);
}
