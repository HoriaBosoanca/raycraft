#include "world.h"
#include <cmath>
#include <iostream>
#include <unordered_map>
#include "chunk.h"

namespace Renderer
{
    std::unordered_map<Vector2, Chunk, Vector2Hash, Vector2Equal> world;
    const int WORLD_SIZE = 6;
    void render_world() {
        for (int x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
            for (int z = -WORLD_SIZE; z <= WORLD_SIZE; z++) {
                world[Vector2((float) x, (float) z)].draw_model(Vector3{(float) x * CHUNK_SIZE, 0.0f, (float) z * CHUNK_SIZE});
            }
        }
    }

    void set_chunk(const Chunk& chunk, const Vector2 chunk_pos) {
        world[chunk_pos] = chunk;
    }

    void build_world_models() {
        for (int x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
            for (int z = -WORLD_SIZE; z <= WORLD_SIZE; z++) {
                const Vector2 chunk_pos((float) x, (float) z);
                world[chunk_pos].build_model(chunk_pos);
            }
        }
    }

    BLOCK get_block(const Vector2 chunk_pos, const Vector3 local_pos) {
        return world[chunk_pos].get_block(local_pos);
    }

    bool is_block_surrounded(const Vector2 chunk_pos, const Vector3 local_pos) {
        Vector2 forward_chunk_pos = local_pos.x == CHUNK_SIZE-1.0f ? Vector2(chunk_pos.x+1.0f, chunk_pos.y) : chunk_pos;
        Vector2 back_chunk_pos = local_pos.x == 0 ? Vector2(chunk_pos.x-1.0f, chunk_pos.y) : chunk_pos;
        Vector2 right_chunk_pos = local_pos.z == CHUNK_SIZE-1.0f ? Vector2(chunk_pos.x, chunk_pos.y+1.0f) : chunk_pos;
        Vector2 left_chunk_pos = local_pos.z == 0 ? Vector2(chunk_pos.x, chunk_pos.y-1.0f) : chunk_pos;

        Vector3 forward_local_pos((int)(local_pos.x+1.0f)%CHUNK_SIZE, local_pos.y, local_pos.z);
        Vector3 back_local_pos(local_pos.x == 0.0f ? CHUNK_SIZE-1.0f : local_pos.x-1.0f, local_pos.y, local_pos.z);
        Vector3 right_local_pos(local_pos.x, local_pos.y, (int)(local_pos.z+1.0f)%CHUNK_SIZE);
        Vector3 left_local_pos(local_pos.x, local_pos.y, local_pos.z == 0.0f ? CHUNK_SIZE-1.0f : local_pos.z-1.0f);

        return
            get_block(forward_chunk_pos, forward_local_pos) != BLOCK::AIR &&
            get_block(back_chunk_pos, back_local_pos) != BLOCK::AIR &&
            get_block(chunk_pos, Vector3(local_pos.x, local_pos.y+1.0f, local_pos.z)) != BLOCK::AIR &&
            get_block(chunk_pos, Vector3(local_pos.x, local_pos.y-1.0f, local_pos.z)) != BLOCK::AIR &&
            get_block(right_chunk_pos, right_local_pos) != BLOCK::AIR &&
            get_block(left_chunk_pos, left_local_pos) != BLOCK::AIR;
    }
}