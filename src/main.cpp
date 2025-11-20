#include "raylib.h"
#include "player.h"
#include "options.h"
#include "physics.h"
#include "textures.h"
#include "world.h"
#include "log.h"
#include "generator/worldgen.h"

void init() {
    Physics::init();
    Renderer::load_resources();
    Generator::generate_world();
    Player::init();
    DisableCursor();
}

void update() {
    options();
    Physics::update();
    Renderer::render_world();
    Player::update();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Raycppcraft");
    init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(Player::camera);
        update();
        EndMode3D();
        log2D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}