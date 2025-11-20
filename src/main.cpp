#include "raylib.h"
#include "camera.h"
#include "options.h"
#include "physics/physics.h"
#include "renderer/textures.h"
#include "renderer/world.h"
#include "log.h"

void setup() {
    Renderer::load_resources();
    Physics::init();
    Renderer::generate_world();
    setup_camera();
    DisableCursor();
}

void update() {
    options();
    Physics::update();
    update_camera();
    Renderer::render_world();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Raycppcraft");
    setup();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        update();
        EndMode3D();
        log2D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}