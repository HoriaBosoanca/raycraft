#include "rbgen.h"
#include "world.h"

namespace RbGen {
	auto* cube_sh = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	void remove_rb(btRigidBody* rb) {
		if (rb != nullptr) {
			World::dynamics_world->removeRigidBody(rb);
			delete rb->getMotionState();
			delete rb->getCollisionShape();
			delete rb;
		}
	}
	btRigidBody* generate_rb(const World::ChunkData& chunk_data, const World::ChunkPos chunk_pos) {
		const auto chunk_shape = new btCompoundShape();
		btTransform t;
		t.setIdentity();
		for (uint32_t x = 0; x < World::U_CHUNK_SIZE; x++) {
			for (uint32_t y = 0; y < World::U_CHUNK_HEIGHT; y++) {
				for (uint32_t z = 0; z < World::U_CHUNK_SIZE; z++) {
					if (const World::LocalPos local_pos = {x, y, z};
						chunk_data.get_block(local_pos) != World::BLOCK::AIR) {
						const World::WorldPos world_pos = chunk_pos + local_pos;
						t.setOrigin(btVector3{static_cast<float>(world_pos.x), static_cast<float>(world_pos.y), static_cast<float>(world_pos.z)});
						chunk_shape->addChildShape(t, cube_sh);
					}
				}
			}
		}
		t.setIdentity();
		const auto chunk_rb = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(
			0.0f,
			new btDefaultMotionState(t),
			chunk_shape
		));
		World::dynamics_world->addRigidBody(chunk_rb);
		return chunk_rb;
	}
}
