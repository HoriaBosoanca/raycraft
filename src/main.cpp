#include "raylib.h"
#include "player.h"
#include "physics.h"
#include "world.h"
#include "textures.h"
#include "log.h"

void setup() {
    Physics::init();
    Player::init();
    World::generate();
    World::build_chunks();
}

void update() {
    Physics::update();
    Player::update();
    World::render();
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