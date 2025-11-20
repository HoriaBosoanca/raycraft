#include "physics.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "raylib.h"

namespace Physics
{
    bool STEP_PHYSICS = false;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    btRigidBody* playerRb;

    void init_player() {
        btCollisionShape* player = new btBoxShape(btVector3(0.5f, 1.0f, 0.5f));
        collisionShapes.push_back(player);
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(btVector3(0.0f, 50.0f, 0.0f));
        constexpr float mass = 3.0f;
        btVector3 localInertia;
        player->calculateLocalInertia(mass, localInertia);
        const btRigidBody::btRigidBodyConstructionInfo rbInfo(
            mass,
            new btDefaultMotionState(transform),
            player,
            localInertia
        );
        playerRb = new btRigidBody(rbInfo);
        playerRb->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
        playerRb->setFriction(0.0f);
        dynamicsWorld->addRigidBody(playerRb);
    }

    void init() {
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
        init_player();
    }

    void update() {
        if (STEP_PHYSICS) {
            dynamicsWorld->stepSimulation(GetFrameTime(), 20);
        }
    }

    void add_static_cube(const btVector3 pos) {
        btCollisionShape* cube = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
        collisionShapes.push_back(cube);
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(pos);
        const btRigidBody::btRigidBodyConstructionInfo rbInfo(
            0.0f,
            new btDefaultMotionState(transform),
            cube,
            btVector3(0.0f, 0.0f, 0.0f)
        );
        dynamicsWorld->addRigidBody(new btRigidBody(rbInfo));
    }

    btVector3 get_player_pos() {
        return playerRb->getWorldTransform().getOrigin();
    }

    btVector3 get_player_velocity() {
        return playerRb->getLinearVelocity();
    }

    void set_player_velocity(const btVector3 vel) {
        playerRb->setLinearVelocity(vel);
    }

    bool is_player_grounded() {
        const btVector3 from = get_player_pos();
        const btVector3 to = from + btVector3(0.0f, -1.0f, 0.0f);
        btCollisionWorld::ClosestRayResultCallback ray(from, to);
        dynamicsWorld->rayTest(from, to, ray);
        return ray.hasHit();
    }
}