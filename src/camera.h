#pragma once

#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "physics/physics.h"

inline Camera camera;

inline void setup_camera() {
    const btVector3 pos = Physics::get_player_pos();
    camera.position = {pos.x(), pos.y(), pos.z()};
    camera.target = {1.0f, 0.0f, 1.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

constexpr float MOVE_SPEED = 20000.0f;
inline void wasd() {
    const btVector3 pos = Physics::get_player_pos();
    const Vector3 diff = Vector3Subtract(camera.target, camera.position);
    camera.position = Vector3(pos.x(), pos.y()+0.5f, pos.z());
    camera.target = Vector3(pos.x()+diff.x, pos.y()+diff.y+0.5f, pos.z()+diff.z);
    const Vector2 raw_direction = Vector2Normalize(Vector2(diff.x, diff.z));
    const Vector2 direction(raw_direction.x*MOVE_SPEED*GetFrameTime(), raw_direction.y*MOVE_SPEED*GetFrameTime());
    if (IsKeyDown(KEY_W)) {
        Physics::add_force_player(btVector3(direction.x, 0.0f, direction.y));
    }
    if (IsKeyDown(KEY_A)) {
        Physics::add_force_player(btVector3(direction.y, 0.0f, -direction.x));
    }
    if (IsKeyDown(KEY_S)) {
        Physics::add_force_player(btVector3(-direction.x, 0.0f, -direction.y));
    }
    if (IsKeyDown(KEY_D)) {
        Physics::add_force_player(btVector3(-direction.y, 0.0f, direction.x));
    }
}

constexpr float JUMP_FORCE = 200000.0f;
inline void jump() {
    if (IsKeyDown(KEY_SPACE) && Physics::is_player_grounded()) {
        const btVector3 vel = Physics::get_player_velocity();
        Physics::set_player_velocity(btVector3(vel.x(), 0.0f, vel.z()));
        Physics::add_force_player(btVector3(0.0f, JUMP_FORCE*GetFrameTime(), 0.0f));
    }
}

constexpr float MAX_VEL_VERTICAL = 5.0f;
constexpr float MAX_VEL_HORIZONTAL = 5.0f;
inline void clamp_velocity() {
    btVector3 vel = Physics::get_player_velocity();
    if (vel.y() > MAX_VEL_VERTICAL) {
        vel.setY(MAX_VEL_VERTICAL);
    }
    if (abs(vel.x()) + abs(vel.z()) > MAX_VEL_HORIZONTAL) {
        // current x / current z = ratio
        // new x / new z = the same ratio => new x = ratio * new z
        // new x + new z = max velocity
        const float ratio = abs(vel.x() / vel.z());
        const float abs_new_z = MAX_VEL_HORIZONTAL/(ratio+1.0f);
        const float abs_new_x = MAX_VEL_HORIZONTAL - abs_new_z;
        vel.setX((vel.x() < 0.0f ? -1.0f : 1.0f) * abs_new_x);
        vel.setZ((vel.z() < 0.0f ? -1.0f : 1.0f) * abs_new_z);
    }
    Physics::set_player_velocity(vel);
}

constexpr float ASCEND_SPEED = 30.0f;
constexpr float FLY_SPEED = 30.0f;
inline void freecam() {
    const Vector3 diff = Vector3Subtract(camera.target, camera.position);
    const Vector2 raw_direction = Vector2Normalize(Vector2(diff.x, diff.z));
    const Vector2 direction(raw_direction.x*FLY_SPEED*GetFrameTime(), raw_direction.y*FLY_SPEED*GetFrameTime());
    if (IsKeyDown(KEY_W)) {
        camera.position += Vector3(direction.x, 0.0f, direction.y);
        camera.target += Vector3(direction.x, 0.0f, direction.y);
    }
    if (IsKeyDown(KEY_A)) {
        camera.position += Vector3(direction.y, 0.0f, -direction.x);
        camera.target += Vector3(direction.y, 0.0f, -direction.x);
    }
    if (IsKeyDown(KEY_S)) {
        camera.position += Vector3(-direction.x, 0.0f, -direction.y);
        camera.target += Vector3(-direction.x, 0.0f, -direction.y);
    }
    if (IsKeyDown(KEY_D)) {
        camera.position += Vector3(-direction.y, 0.0f, direction.x);
        camera.target += Vector3(-direction.y, 0.0f, direction.x);
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

inline void update_camera() {
    if (Physics::STEP_PHYSICS) {
        wasd();
        jump();
        clamp_velocity();
    } else {
        freecam();
    }
    UpdateCamera(&camera, CAMERA_FIRST_PERSON);
}