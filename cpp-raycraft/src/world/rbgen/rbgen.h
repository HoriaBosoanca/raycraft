#pragma once

#include "btBulletDynamicsCommon.h"
#include "chunk.h"

namespace RbGen {
	void remove_rb(btRigidBody* rb);
	btRigidBody* generate_rb(const World::ChunkData& chunk_data, World::ChunkPos chunk_pos);
}