#include "raylib.h"
#include "player.h"
#include "world.h"
#include "log.h"

void setup() {
    World::setup();
    Player::setup();
}

void update() {
    World::update();
    Player::update();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Raycppcraft");
    setup();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(Player::camera);
        update();
        EndMode3D();
        draw_debug_menu();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}