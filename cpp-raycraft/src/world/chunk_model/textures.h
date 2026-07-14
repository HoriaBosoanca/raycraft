#pragma once

#include <map>
#include <array>
#include "chunk.h"
#include "raylib.h"

namespace ChunkModel
{
    inline Mesh cube;
    inline Material default_mat;
    constexpr float ATLAS_UNIT = 1.0f/8.0f;
    inline Texture2D ATLAS;

    inline void load_textures() {
        ATLAS = LoadTexture("assets/atlas.png");
        default_mat = LoadMaterialDefault();
        cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    }

    inline auto AIR = Vector2{2.0f, 6.0f},
                GRASS_TOP = Vector2{0.0f, 7.0f},
                GRASS_SIDE = Vector2{0.0f, 6.0f},
                DIRT = Vector2{1.0f, 7.0f},
                STONE = Vector2{4.0f, 6.0f},
                OAK_PLANKS = Vector2{7.0f, 7.0f},
                OAK_LOG_SIDE = Vector2{6.0f, 6.0f},
                OAK_LOG_TOP = Vector2{6.0f, 7.0f},
                OAK_LEAVES = Vector2{7.0f, 6.0f};

    inline std::map<World::BLOCK, std::array<Vector2, 6>> t_map = {
        {World::BLOCK::AIR,   {AIR, AIR, AIR, AIR, AIR, AIR}},
        {World::BLOCK::GRASS, {GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE}},
        {World::BLOCK::DIRT,  {DIRT, DIRT, DIRT, DIRT, DIRT, DIRT}},
        {World::BLOCK::STONE, {STONE, STONE, STONE, STONE, STONE, STONE}},
        {World::BLOCK::OAK_PLANKS, {OAK_PLANKS, OAK_PLANKS, OAK_PLANKS, OAK_PLANKS, OAK_PLANKS, OAK_PLANKS}},
        {World::BLOCK::OAK_LOG, {OAK_LOG_SIDE, OAK_LOG_SIDE, OAK_LOG_TOP, OAK_LOG_TOP, OAK_LOG_SIDE, OAK_LOG_SIDE}},
        {World::BLOCK::OAK_LEAVES, {OAK_LEAVES, OAK_LEAVES, OAK_LEAVES, OAK_LEAVES, OAK_LEAVES, OAK_LEAVES}}
    };
}
