#include <iostream>

#include "chunk.h"
#include "FastNoiseLite.h"
#include "world.h"
#include <random>

namespace WorldGen
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution rand_chance(0.0f, 100.0f);
    bool random(const float chance) {
        return rand_chance(gen) < chance;
    }

    constexpr float TREE_CHANCE = 0.1f;

    constexpr int32_t TREE_H = 7,
                      TREE_LEAF_W = 2,
                      TREE_LEAF_H = 2;
    void generate_tree(World::WorldPos world_pos) {
        for (const auto end_y = world_pos.y + TREE_H; world_pos.y < end_y; world_pos.y++) {
            World::set_block(world_pos, World::BLOCK::OAK_LOG);
        }
        for (auto x = world_pos.x-TREE_LEAF_W; x <= world_pos.x+TREE_LEAF_W; x++) {
            for (auto y = world_pos.y-TREE_LEAF_H-1; y < world_pos.y-1; y++) {
                for (auto z = world_pos.z-TREE_LEAF_W; z <= world_pos.z+TREE_LEAF_W; z++) {
                    if (const auto leaf_pos = World::WorldPos{x, y, z}; World::get_block(leaf_pos) != World::BLOCK::OAK_LOG) {
                        World::set_block(leaf_pos, World::BLOCK::OAK_LEAVES);
                    }
                }
            }
        }

        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.x-=1;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.y-=1;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.x+=2;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.y+=1;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.x-=1;
        world_pos.z-=1;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.y-=1;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.z+=2;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
        world_pos.y+=1;
        World::set_block(world_pos, World::BLOCK::OAK_LEAVES);
    }

    constexpr float TERRAIN_FREQUENCY = 1.5f;
    void generate_world() {
        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        for (int chunk_x = -World::WORLD_SIZE; chunk_x <= World::WORLD_SIZE; chunk_x++) {
            for (int chunk_z = -World::WORLD_SIZE; chunk_z <= World::WORLD_SIZE; chunk_z++) {
                for (uint32_t local_x = 0; local_x < World::U_CHUNK_SIZE; local_x++) {
                    for (uint32_t local_z = 0; local_z < World::U_CHUNK_SIZE; local_z++) {
                        World::WorldPos world_pos = World::ChunkPos{chunk_x, chunk_z} + World::LocalPos{local_x, 0, local_z};
                        const auto grass_height = static_cast<int32_t>((noise.GetNoise(static_cast<float>(world_pos.x)*TERRAIN_FREQUENCY, static_cast<float>(world_pos.z)*TERRAIN_FREQUENCY) + 1.0f)
                            / 2.0f * static_cast<float>(World::CHUNK_HEIGHT-1) / 2.0f);
                        int32_t y = 0;
                        for (; y < grass_height; y++) {
                            world_pos.y = y;
                            World::set_block(world_pos, y < (grass_height-1)/2 ? World::BLOCK::STONE : World::BLOCK::DIRT);
                        }
                        world_pos.y = y;
                        World::set_block(world_pos, World::BLOCK::GRASS);
                        if (random(TREE_CHANCE)) {
                            generate_tree(world_pos);
                        }
                    }
                }
            }
        }
    }
}