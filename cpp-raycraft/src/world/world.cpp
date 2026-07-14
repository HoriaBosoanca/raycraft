#include "world.h"
#include "btBulletDynamicsCommon.h"
#include <ranges>
#include "chunk.h"
#include "rbgen.h"
#include "renderer.h"
#include "worldgen.h"

namespace World
{
    std::map<ChunkPos, Chunk> world;
    btDynamicsWorld* dynamics_world;

    constexpr float GRAVITY = -20.0f;
    void setup_dynamics_world() {
        auto* cfg = new btDefaultCollisionConfiguration();
        dynamics_world = new btDiscreteDynamicsWorld(new btCollisionDispatcher(cfg), new btDbvtBroadphase(), new btSequentialImpulseConstraintSolver(), cfg);
        dynamics_world->setGravity(btVector3(0.0f, GRAVITY, 0.0f));
    }
    void build_chunk(const ChunkPos chunk_pos) {
        if (world.contains(chunk_pos)) {
            auto& [chunk_data, chunk_model, chunk_rb] = world[chunk_pos];
            chunk_model.rebuild_model(chunk_data, chunk_pos);
            RbGen::remove_rb(chunk_rb);
            chunk_rb = RbGen::generate_rb(chunk_data, chunk_pos);
        }
    }
    void build_chunks() {
        for (auto& chunk_pos : world | std::ranges::views::keys) {
            build_chunk(chunk_pos);
        }
    }
    void setup() {
        ChunkModel::load_textures();
        WorldGen::generate_world();
        setup_dynamics_world();
        build_chunks();
    }

    Renderer::render_vec rv;
    void render() {
        for (auto& [chunk_pos, chunk] : world) {
            rv.emplace_back(&chunk.chunk_model, chunk_pos);
        }
        Renderer::render(rv);
        rv.clear();
    }
    bool STEP_PHYSICS = true;
    void update_dynamics_world() {
        if (STEP_PHYSICS) {
            dynamics_world->stepSimulation(GetFrameTime(), 20);
        }
    }
    void update() {
        update_dynamics_world();
        render();
    }

    void update_chunk_around_block(WorldPos world_pos) {
        const auto chunk_pos = world_pos.get_chunk_pos();
        build_chunk(chunk_pos);

        world_pos.x+=1;
        if (world_pos.get_chunk_pos() != chunk_pos)
            build_chunk(world_pos.get_chunk_pos());
        world_pos.x-=2;
        if (world_pos.get_chunk_pos() != chunk_pos)
            build_chunk(world_pos.get_chunk_pos());
        world_pos.x+=1;
        world_pos.z+=1;
        if (world_pos.get_chunk_pos() != chunk_pos)
            build_chunk(world_pos.get_chunk_pos());
        world_pos.z-=2;
        if (world_pos.get_chunk_pos() != chunk_pos)
            build_chunk(world_pos.get_chunk_pos());
    }
    BLOCK get_block(const WorldPos world_pos) {
        return
            world.contains(world_pos.get_chunk_pos()) ?
                world[world_pos.get_chunk_pos()].chunk_data.get_block(world_pos.get_local_pos()) :
                BLOCK::AIR;
    }
    void set_block(const WorldPos world_pos, const BLOCK block) {
        world[world_pos.get_chunk_pos()].chunk_data.set_block(world_pos.get_local_pos(), block);
    }
    bool is_opaque(const BLOCK block) {
        return
            block != BLOCK::AIR &&
            block != BLOCK::OAK_LEAVES;
    }
    bool is_block_surrounded(const WorldPos world_pos) {
        return
            is_opaque(get_block(world_pos+WorldPos{1, 0, 0})) &&
            is_opaque(get_block(world_pos+WorldPos{-1, 0, 0})) &&
            is_opaque(get_block(world_pos+WorldPos{0, 1, 0})) &&
            is_opaque(get_block(world_pos+WorldPos{0, -1, 0})) &&
            is_opaque(get_block(world_pos+WorldPos{0, 0, 1})) &&
            is_opaque(get_block(world_pos+WorldPos{0, 0, -1}));
    }
}
