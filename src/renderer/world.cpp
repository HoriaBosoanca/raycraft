#include "world.h"
#include <unordered_map>
#include "chunk.h"

namespace Renderer
{
    std::unordered_map<Vector2, Chunk, Vector2Hash, Vector2Equal> world;
    const int WORLD_SIZE = 6;
    void render_world() {
        for (int x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
            for (int z = -WORLD_SIZE; z <= WORLD_SIZE; z++) {
                world[Vector2{(float) x, (float) z}].draw_model(Vector3{(float) x * CHUNK_SIZE, 0.0f, (float) z * CHUNK_SIZE});
            }
        }
    }

    void add_chunk(Chunk& chunk, const Vector2 pos) {
        chunk.build_model();
        world[pos] = chunk;
    }
}