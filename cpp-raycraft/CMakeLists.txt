cmake_minimum_required(VERSION 3.31)
project(raycppcraft)
set(CMAKE_CXX_STANDARD 23)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()

# my project
file(GLOB SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/world/chunk_model/*.cpp
    src/world/rbgen/*.cpp
    src/world/renderer/*.cpp
    src/world/*.cpp
    src/player/*.cpp
    src/worldgen/*.cpp
)
add_executable(raycppcraft ${SOURCES})
target_include_directories(raycppcraft PRIVATE
        src
        src/world
        src/world/chunk_model
        src/world/rbgen
        src/world/renderer
        src/worldgen
        src/player
)
set_target_properties(raycppcraft PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# deps:
#   raylib
add_subdirectory(lib/raylib-5.5)
target_link_libraries(raycppcraft raylib_static)
#   FastNoiseLite
target_include_directories(raycppcraft PRIVATE lib/FastNoiseLite/include)
#   bullet
add_subdirectory(lib/bullet3-3.25)
target_include_directories(raycppcraft PRIVATE lib/bullet3-3.25/src)
target_link_libraries(raycppcraft BulletDynamics BulletCollision LinearMath)
#   windows deps
if (WIN32)
    target_link_libraries(raycppcraft winmm)
endif ()