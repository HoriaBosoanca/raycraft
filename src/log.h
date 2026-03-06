#pragma once

#include <cstdint>
#include <processthreadsapi.h>
#include <psapi.h>

inline int64_t getMemoryKB() {
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return static_cast<int64_t>(info.WorkingSetSize) / 1024;
}

inline void draw_debug_menu() {
    DrawText("F9 toggle freecam, F10 toggle cursor, F11 toggle fullscreen", 10, 30, 20, BLACK);
    DrawText(TextFormat("FPS: %d\nMemory:\n%d KB\n%d MB",
        GetFPS(),
        getMemoryKB(),
        getMemoryKB()/1024
        ), 10, 60, 20, BLACK);
    const btVector3 pos = Physics::get_player_pos();
    DrawText(TextFormat("X: %f,\nY: %f,\nZ: %f", pos.x(), pos.y(), pos.z()), 10, 150, 20, BLACK);
}