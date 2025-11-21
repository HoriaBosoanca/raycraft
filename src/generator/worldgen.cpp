#include "chunk.h"
#include "FastNoiseLite.h"
#include "physics.h"
#include "world.h"
#include "LinearMath/btVector3.h"

namespace Generator
{
    FastNoiseLite noise;

    void generate_chunk(Renderer::Chunk& chunk, const Vector2 world_pos) {
        for (int x = 0; x < Renderer::CHUNK_SIZE; x++) {
            for (int z = 0; z < Renderer::CHUNK_SIZE; z++) {
                const int height = (int) ((noise.GetNoise(world_pos.x* Renderer::CHUNK_SIZE + (float) x,
                    world_pos.y* Renderer::CHUNK_SIZE + (float) z)+1.0f)/2.0f*(float)Renderer::CHUNK_HEIGHT);
                int y = 0;
                for (; y < height-1; y++) {
                    chunk.set_block(Vector3{(float) x, (float) y, (float) z}, y < (height-1)/2 ? Renderer::BLOCK::STONE : Renderer::BLOCK::DIRT);
                }
                chunk.set_block(Vector3{(float) x, (float) y, (float) z}, Renderer::BLOCK::GRASS);
                Physics::add_static_cube(btVector3(world_pos.x* Renderer::CHUNK_SIZE + (float) x,
                    (float) y,
                    world_pos.y* Renderer::CHUNK_SIZE + (float) z));
            }
        }
    }

    void generate_world() {
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        for (int x = -Renderer::WORLD_SIZE; x <= Renderer::WORLD_SIZE; x++) {
            for (int z = -Renderer::WORLD_SIZE; z <= Renderer::WORLD_SIZE; z++) {
                Renderer::Chunk chunk;
                const Vector2 chunk_pos((float) x, (float) z);
                generate_chunk(chunk, chunk_pos);
                Renderer::set_chunk(chunk, chunk_pos);
            }
        }
    }
}