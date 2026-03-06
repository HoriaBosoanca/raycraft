#include "physics.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "chunk.h"
#include "raylib.h"

namespace Physics
{
    bool STEP_PHYSICS = false;

    btDiscreteDynamicsWorld* dynamicsWorld;
    auto world_sh = new btCompoundShape();
    btRigidBody* player_rb;
    btCollisionShape* cube_sh = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

    void init_dyna_world() {
        auto* cfg = new btDefaultCollisionConfiguration();
        dynamicsWorld = new btDiscreteDynamicsWorld(new btCollisionDispatcher(cfg), new btDbvtBroadphase(), new btSequentialImpulseConstraintSolver(), cfg);
        dynamicsWorld->setGravity(btVector3(0.0f, -20.0f, 0.0f));
    }

    void init_player_rb() {
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(0.0f, 50.0f, 0.0f));
        const btRigidBody::btRigidBodyConstructionInfo rbInfo(
            1.0f,
            new btDefaultMotionState(t),
            new btBoxShape(btVector3(0.5f, 1.0f, 0.5f))
        );
        player_rb = new btRigidBody(rbInfo);
        player_rb->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
        player_rb->setFriction(0.0f);
        dynamicsWorld->addRigidBody(player_rb);
    }

    void init_world_rb() {
        btTransform t;
        t.setIdentity();
        dynamicsWorld->addRigidBody(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(
            0.0f,
            new btDefaultMotionState(t),
            world_sh
        )));
    }

    void setup() {
        init_dyna_world();
        init_world_rb();
        init_player_rb();
    }

    void update() {
        if (STEP_PHYSICS) {
            dynamicsWorld->stepSimulation(GetFrameTime(), 20);
        }
    }

    void add_block(const Renderer::WorldPos world_pos) {
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3{static_cast<float>(world_pos.x), static_cast<float>(world_pos.y), static_cast<float>(world_pos.z)});
        world_sh->addChildShape(t, cube_sh);
    }

    btVector3 get_player_pos() {
        return player_rb->getWorldTransform().getOrigin();
    }

    btVector3 get_player_velocity() {
        return player_rb->getLinearVelocity();
    }

    void set_player_velocity(const btVector3 vel) {
        player_rb->setLinearVelocity(vel);
    }

    bool is_player_grounded() {
        const btVector3 from = get_player_pos();
        const btVector3 to = from + btVector3(0.0f, -1.0f, 0.0f);
        btCollisionWorld::ClosestRayResultCallback ray(from, to);
        dynamicsWorld->rayTest(from, to, ray);
        return ray.hasHit();
    }
}