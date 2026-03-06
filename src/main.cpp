#include "raylib.h"
#include "player.h"
#include "physics.h"
#include "textures.h"
#include "world.h"
#include "log.h"
#include "worldgen.h"

void setup() {
    WorldGen::generate_world();
    Physics::setup();
    Renderer::load_textures();
    Renderer::build_world_models();
    Player::init();
}

void update() {
    Player::update_options();
    Player::update();
    Physics::update();
    Renderer::render_world();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Raycppcraft");
    setup();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(Player::camera);
        update();
        EndMode3D();
        draw_debug_menu();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}