cmake_minimum_required(VERSION 3.31)
project(raycppcraft)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
file(GLOB SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/renderer/*.cpp
        src/physics/*.cpp
        src/player/*.cpp
        src/generator/*.cpp
)
add_executable(raycppcraft ${SOURCES})
add_subdirectory(lib/raylib-5.5)
add_subdirectory(lib/bullet3-3.25)
target_include_directories(raycppcraft PRIVATE
        src
        src/renderer
        src/physics
        src/player
        src/generator
        lib/FastNoiseLite/include
        lib/FastNoiseLite/include/physics
        lib/FastNoiseLite/include/renderer
        lib/bullet3-3.25/src
)
target_link_libraries(raycppcraft
        raylib_static
        BulletDynamics BulletCollision LinearMath
        winmm
)