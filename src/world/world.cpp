#include "world.h"

#include <iostream>
#include <ranges>
#include "chunk.h"
#include "rbgen.h"
#include "worldgen.h"

namespace World
{
    std::map<ChunkPos, Chunk> world;

    void generate() {
        MeshGen::load_textures();
        WorldGen::generate_world();
    }
    BLOCK get_block(const WorldPos world_pos) {
        return world[world_pos.get_chunk_pos()].chunk_data.get_block(world_pos.get_local_pos());
    }
    void set_block(const WorldPos world_pos, const BLOCK block) {
        world[world_pos.get_chunk_pos()].chunk_data.set_block(world_pos.get_local_pos(), block);
    }
    void render() {
        for (const auto& [chunk_pos, chunk] : world) {
            chunk.chunk_model.render(chunk_pos);
        }
    }
    void build_chunk(const ChunkPos chunk_pos) {
        Chunk& chunk = world[chunk_pos];
        chunk.chunk_model.build_model(chunk.chunk_data, chunk_pos);
        RbGen::remove_rb(chunk.chunk_rb);
        chunk.chunk_rb = RbGen::generate_rb(chunk.chunk_data, chunk_pos);
    }
    void build_chunks() {
        for (auto& chunk_pos : world | std::ranges::views::keys) {
            build_chunk(chunk_pos);
        }
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
