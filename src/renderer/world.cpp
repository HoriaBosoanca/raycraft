#include "world.h"
#include <unordered_map>
#include "../renderer/chunk.h"
#include "FastNoiseLite.h"
#include "physics/physics.h"

namespace Renderer
{
    struct Vector2Hash {
        size_t operator()(const Vector2& v) const noexcept {
            return std::hash<float>{}(v.x) ^ (std::hash<float>{}(v.y) << 1);
        }
    };
    struct Vector2Equal {
        bool operator()(const Vector2& a, const Vector2& b) const noexcept {
            return a.x == b.x && a.y == b.y;
        }
    };
    std::unordered_map<Vector2, Chunk, Vector2Hash, Vector2Equal> world;
#define WORLD_SIZE 6
    FastNoiseLite noise;

    void generate_chunk(Chunk& chunk, const Vector2 world_pos) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                const int height = (int) ((noise.GetNoise(world_pos.x*CHUNK_SIZE + x, world_pos.y*CHUNK_SIZE + z)+1.0f)/2.0f*(float)CHUNK_HEIGHT);
                int y = 0;
                for (; y < height-1; y++) {
                    chunk.add_block(Vector3{(float) x, (float) y, (float) z}, y < (height-1)/2 ? BLOCK::STONE : BLOCK::DIRT);
                }
                chunk.add_block(Vector3{(float) x, (float) y, (float) z}, BLOCK::GRASS);
                Physics::add_static_cube(btVector3(world_pos.x*CHUNK_SIZE + (float) x, (float) y, world_pos.y*CHUNK_SIZE + (float) z));
            }
        }
    }

    void generate_world() {
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        for (int x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
            for (int z = -WORLD_SIZE; z <= WORLD_SIZE; z++) {
                Chunk chunk;
                Vector2 world_pos = Vector2{(float) x, (float) z};
                generate_chunk(chunk, world_pos);
                chunk.build_model();
                world[world_pos] = chunk;
            }
        }
    }

    void render_world() {
        for (int x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
            for (int z = -WORLD_SIZE; z <= WORLD_SIZE; z++) {
                world[Vector2{(float) x, (float) z}].draw_model(Vector3{(float) x * CHUNK_SIZE, 0.0f, (float) z * CHUNK_SIZE});
            }
        }
    }
}