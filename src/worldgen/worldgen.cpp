#include "chunk.h"
#include "FastNoiseLite.h"
#include "physics.h"
#include "world.h"

namespace WorldGen
{
    FastNoiseLite noise;

    void generate_chunk(Renderer::Chunk& chunk, const Renderer::ChunkPos chunk_pos) {
        for (uint32_t x = 0; x < Renderer::U_CHUNK_SIZE; x++) {
            for (uint32_t z = 0; z < Renderer::U_CHUNK_SIZE; z++) {
                Renderer::WorldPos world_pos = chunk_pos + Renderer::LocalPos{x, 0, z};
                const auto grass_height = static_cast<uint32_t>((noise.GetNoise(static_cast<float>(world_pos.x), static_cast<float>(world_pos.z)) + 1.0f)
                    / 2.0f * static_cast<float>(Renderer::CHUNK_HEIGHT-1));
                uint32_t y = 0;
                for (; y < grass_height; y++)
                    chunk.set_block({x, y, z}, y < (grass_height-1)/2 ? Renderer::BLOCK::STONE : Renderer::BLOCK::DIRT);
                chunk.set_block({x, y, z}, Renderer::BLOCK::GRASS);
                world_pos.y = static_cast<int32_t>(y);
                Physics::add_block(world_pos);
            }
        }
    }

    void generate_world() {
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        for (int x = -Renderer::WORLD_SIZE; x <= Renderer::WORLD_SIZE; x++) {
            for (int z = -Renderer::WORLD_SIZE; z <= Renderer::WORLD_SIZE; z++) {
                Renderer::Chunk chunk;
                const Renderer::ChunkPos chunk_pos{x, z};
                generate_chunk(chunk, chunk_pos);
                Renderer::add_chunk_to_world(chunk, chunk_pos);
            }
        }
    }
}