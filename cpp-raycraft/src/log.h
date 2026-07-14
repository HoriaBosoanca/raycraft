#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <processthreadsapi.h>
#include <psapi.h>
inline int64_t getMemoryKB() {
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return static_cast<int64_t>(info.WorkingSetSize) / 1024;
}
#else
inline int64_t getMemoryKB() {
    std::ifstream status("/proc/self/status");
    std::string line;
    while (std::getline(status, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            std::istringstream iss(line);
            std::string key, unit; int64_t value;
            iss >> key >> value >> unit;
            return value;
        }
    }
    return 0;
}
#endif

inline void draw_debug_menu() {
    DrawText("F9 toggle freecam, F10 toggle cursor, F11 toggle fullscreen", 10, 30, 20, BLACK);
    DrawText(TextFormat("FPS: %d\nMemory:\n%d KB\n%d MB",
        GetFPS(),
        getMemoryKB(),
        getMemoryKB()/1024
        ), 10, 60, 20, BLACK);
    const auto [x, y, z] = Player::camera.position;
    DrawText(TextFormat("X: %f,\nY: %f,\nZ: %f", x, y, z), 10, 150, 20, BLACK);
}