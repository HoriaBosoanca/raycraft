#include "chunk.h"
#include "FastNoiseLite.h"
#include "physics.h"
#include "world.h"

namespace WorldGen
{
    void generate_world() {
        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        for (int chunk_x = -World::WORLD_SIZE; chunk_x <= World::WORLD_SIZE; chunk_x++) {
            for (int chunk_z = -World::WORLD_SIZE; chunk_z <= World::WORLD_SIZE; chunk_z++) {
                for (uint32_t local_x = 0; local_x < World::U_CHUNK_SIZE; local_x++) {
                    for (uint32_t local_z = 0; local_z < World::U_CHUNK_SIZE; local_z++) {
                        World::WorldPos world_pos = World::ChunkPos{chunk_x, chunk_z} + World::LocalPos{local_x, 0, local_z};
                        const auto grass_height = static_cast<int32_t>((noise.GetNoise(static_cast<float>(world_pos.x), static_cast<float>(world_pos.z)) + 1.0f)
                            / 2.0f * static_cast<float>(World::CHUNK_HEIGHT-1));
                        int32_t y = 0;
                        for (; y < grass_height; y++) {
                            world_pos.y = y;
                            World::set_block(world_pos, y < (grass_height-1)/2 ? World::BLOCK::STONE : World::BLOCK::DIRT);
                        }
                        world_pos.y = y;
                        World::set_block(world_pos, World::BLOCK::GRASS);
                    }
                }
            }
        }
    }
}