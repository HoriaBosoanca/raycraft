#pragma once

inline bool cursor_enabled = false;

inline void options() {
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