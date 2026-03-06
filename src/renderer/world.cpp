#include "world.h"
#include <unordered_map>
#include "chunk.h"

namespace Renderer
{
    std::map<ChunkPos, Chunk> world;
    const int32_t WORLD_SIZE = 6;
    void render_world() {
        for (int32_t x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
            for (int32_t z = -WORLD_SIZE; z <= WORLD_SIZE; z++) {
                world[{x, z}].draw_model({x * CHUNK_SIZE, 0, z * CHUNK_SIZE});
            }
        }
    }

    void add_chunk_to_world(const Chunk& chunk, const ChunkPos chunk_pos) {
        world[chunk_pos] = chunk;
    }

    void build_world_models() {
        for (int32_t x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
            for (int32_t z = -WORLD_SIZE; z <= WORLD_SIZE; z++) {
                world[{x, z}].build_model({x, z});
            }
        }
    }

    BLOCK get_block(const WorldPos world_pos) {
        return world[world_pos.get_chunk_pos()].get_block(world_pos.get_local_pos());
    }

    bool is_block_surrounded(const WorldPos world_pos) {
        return
            get_block(world_pos+WorldPos{1, 0, 0}) != BLOCK::AIR &&
            get_block(world_pos+WorldPos{-1, 0, 0}) != BLOCK::AIR &&
            get_block(world_pos+WorldPos{0, 1, 0}) != BLOCK::AIR &&
            get_block(world_pos+WorldPos{0, -1, 0}) != BLOCK::AIR &&
            get_block(world_pos+WorldPos{0, 0, 1}) != BLOCK::AIR &&
            get_block(world_pos+WorldPos{0, 0, -1}) != BLOCK::AIR;
    }
}