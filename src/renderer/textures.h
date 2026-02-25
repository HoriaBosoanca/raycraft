#pragma once

#include <map>
#include <array>
#include <cstdint>

namespace Renderer
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

    inline auto AIR = Vector2{2.0f, 6.0f};
    inline auto GRASS_TOP = Vector2{0.0f, 7.0f};
    inline auto GRASS_SIDE = Vector2{0.0f, 6.0f};
    inline auto DIRT = Vector2{1.0f, 7.0f};
    inline auto STONE = Vector2{4.0f, 6.0f};

    enum class BLOCK : uint8_t {
        AIR,
        GRASS,
        DIRT,
        STONE
    };

    inline std::map<BLOCK, std::array<Vector2, 6>> t_map = {
        {BLOCK::AIR,   {AIR, AIR, AIR, AIR, AIR, AIR}},
        {BLOCK::GRASS, {GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE}},
        {BLOCK::DIRT,  {DIRT, DIRT, DIRT, DIRT, DIRT, DIRT}},
        {BLOCK::STONE, {STONE, STONE, STONE, STONE, STONE, STONE}}
    };
}