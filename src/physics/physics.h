#pragma once

#include "btBulletDynamicsCommon.h"

namespace Physics
{
    extern bool STEP_PHYSICS;
    void init();
    void update();
    void add_static_cube(btVector3 pos);
    btVector3 get_player_pos();
    btVector3 get_player_velocity();
    void set_player_velocity(btVector3 vel);
    bool is_player_grounded();
}
