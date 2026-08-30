cmake_minimum_required(VERSION 4.1)
project(raycppcraft)
set(CMAKE_CXX_STANDARD 23)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()
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
set_target_properties(raycppcraft PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    fastnoiselite
    GIT_REPOSITORY https://github.com/Auburn/FastNoiseLite
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    bullet3
    GIT_REPOSITORY https://github.com/bulletphysics/bullet3
    GIT_TAG 3.25
    GIT_SHALLOW TRUE
)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_PYBULLET OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib fastnoiselite bullet3)
target_include_directories(raycppcraft PRIVATE
    ${fastnoiselite_SOURCE_DIR}/Cpp
    ${bullet3_SOURCE_DIR}/src
)
target_link_libraries(raycppcraft PRIVATE raylib BulletDynamics BulletCollision LinearMath)
if (WIN32)
    target_link_libraries(raycppcraft PRIVATE winmm)
endif ()