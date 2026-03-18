#pragma once

#include <vector>
#include "chunk_model.h"

namespace Renderer
{
	using render_pair = std::pair<ChunkModel::ChunkModel*, World::ChunkPos>;
	using render_vec  = std::vector<render_pair>;
	void render(render_vec& rv);
}
