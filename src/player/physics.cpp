#include "player.h"
#include "world.h"

namespace Player
{
    constexpr float W_PLAYER = 0.45f,
                    W_WALK_PLAYER = 0.40f,
                    H_PLAYER = 1.0f;
    btRigidBody* player_rb;

    void setup_player_rb() {
        btTransform t;
        t.setIdentity();
        const auto [x, y, z] = camera.position;
        t.setOrigin(btVector3(x, y, z));
        const btRigidBody::btRigidBodyConstructionInfo rbInfo(
            1.0f,
            new btDefaultMotionState(t),
            new btBoxShape(btVector3(W_PLAYER, H_PLAYER, W_PLAYER))
        );
        player_rb = new btRigidBody(rbInfo);
        player_rb->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
        player_rb->setFriction(0.0f);
        player_rb->setActivationState(DISABLE_DEACTIVATION);
        World::dynamics_world->addRigidBody(player_rb);
    }

    btVector3 get_physics_player_pos() {
        return player_rb->getWorldTransform().getOrigin();
    }

    btVector3 get_player_velocity() {
        return player_rb->getLinearVelocity();
    }

    void set_player_velocity(const btVector3 vel) {
        player_rb->setLinearVelocity(vel);
    }

    btVector3 offsets[4] = {{-W_WALK_PLAYER, 0.0f, -W_WALK_PLAYER},
                            {-W_WALK_PLAYER, 0.0f, W_WALK_PLAYER},
                            {W_WALK_PLAYER, 0.0f, -W_WALK_PLAYER},
                            {W_WALK_PLAYER, 0.0f, W_WALK_PLAYER}};
    bool is_player_grounded() {
        const btVector3 player = get_physics_player_pos();
        for (btVector3& offset : offsets) {
            const btVector3 from = player + offset;
            const btVector3 to = from + btVector3(0.0f, -1.0f, 0.0f);
            btCollisionWorld::ClosestRayResultCallback ray(from, to);
            World::dynamics_world->rayTest(from, to, ray);
            if (ray.hasHit()) return true;
        }
        return false;
    }

    void player_set_target_block(const World::BLOCK block, const btVector3 from, btVector3 to, const float reach_dist, const bool replace) {
        btVector3 dir = to-from;
        dir.normalize();
        to = from+dir*reach_dist;
        btCollisionWorld::ClosestRayResultCallback ray(from, to);
        World::dynamics_world->rayTest(from, to, ray);
        if (ray.hasHit()) {
            const auto hit = ray.m_hitPointWorld + (replace ? -1.0f : 1.0f) * ray.m_hitNormalWorld * 0.1f;
            const World::WorldPos world_pos = {static_cast<int32_t>(round(hit.x())), static_cast<int32_t>(round(hit.y())), static_cast<int32_t>(round(hit.z()))};
            World::set_block(world_pos, block);
            World::update_chunk_around_block(world_pos);
        }
    }
}