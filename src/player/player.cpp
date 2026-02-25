#include "raylib.h"
#include "raymath.h"
#include "physics.h"

namespace Player
{
    Camera camera;

    void init() {
        DisableCursor();
        const btVector3 pos = Physics::get_player_pos();
        camera.position = {pos.x(), pos.y(), pos.z()};
        camera.target = {1.0f, 0.0f, 1.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 90.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    constexpr float JUMP_FORCE = 7.0f;
    constexpr float MOVE_SPEED = 3.5f;
    void move() {
        const btVector3 pos = Physics::get_player_pos();
        const auto [off_X, off_y, off_z] = camera.target - camera.position;
        camera.position = Vector3(pos.x(), pos.y()+0.5f, pos.z());
        camera.target = Vector3(pos.x()+off_X, pos.y()+off_y+0.5f, pos.z()+off_z);
        const auto [x_dir, y_dir] = Vector2Normalize(Vector2(off_X, off_z));
        Vector2 mov{};
        if (IsKeyDown(KEY_W)) {
            mov += Vector2(x_dir, y_dir);
        }
        if (IsKeyDown(KEY_A)) {
            mov += Vector2(y_dir, -x_dir);
        }
        if (IsKeyDown(KEY_S)) {
            mov += Vector2(-x_dir, -y_dir);
        }
        if (IsKeyDown(KEY_D)) {
            mov += Vector2(-y_dir, x_dir);
        }
        mov = Vector2Normalize(mov) * MOVE_SPEED;
        const btVector3 final_mov(mov.x,
            IsKeyDown(KEY_SPACE) && Physics::is_player_grounded() ? JUMP_FORCE : Physics::get_player_velocity().y(),
            mov.y);
        Physics::set_player_velocity(final_mov);
    }

    constexpr float ASCEND_SPEED = 30.0f;
    constexpr float FLY_SPEED = 30.0f;
    void freecam() {
        const Vector3 diff = camera.target - camera.position;
        const Vector2 dir = Vector2Normalize(Vector2(diff.x, diff.z))*FLY_SPEED*GetFrameTime();
        if (IsKeyDown(KEY_W)) {
            camera.position += Vector3(dir.x, 0.0f, dir.y);
            camera.target += Vector3(dir.x, 0.0f, dir.y);
        }
        if (IsKeyDown(KEY_A)) {
            camera.position += Vector3(dir.y, 0.0f, -dir.x);
            camera.target += Vector3(dir.y, 0.0f, -dir.x);
        }
        if (IsKeyDown(KEY_S)) {
            camera.position += Vector3(-dir.x, 0.0f, -dir.y);
            camera.target += Vector3(-dir.x, 0.0f, -dir.y);
        }
        if (IsKeyDown(KEY_D)) {
            camera.position += Vector3(-dir.y, 0.0f, dir.x);
            camera.target += Vector3(-dir.y, 0.0f, dir.x);
        }
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            camera.position += Vector3(0.0f, -ASCEND_SPEED*GetFrameTime(), 0.0f);
            camera.target += Vector3(0.0f, -ASCEND_SPEED*GetFrameTime(), 0.0f);
        }
        if (IsKeyDown(KEY_SPACE)) {
            camera.position += Vector3(0.0f, ASCEND_SPEED*GetFrameTime(), 0.0f);
            camera.target += Vector3(0.0f, ASCEND_SPEED*GetFrameTime(), 0.0f);
        }
    }

    void update() {
        if (Physics::STEP_PHYSICS) {
            move();
        } else {
            freecam();
        }
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
    }

    bool cursor_enabled = false;
    void update_options() {
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }
        if (IsKeyPressed(KEY_F10)) {
            if (cursor_enabled) {
                DisableCursor();
            } else {
                EnableCursor();
            }
            cursor_enabled = !cursor_enabled;
        }
        if (IsKeyPressed(KEY_F9)) {
            Physics::STEP_PHYSICS = !Physics::STEP_PHYSICS;
        }
    }
}