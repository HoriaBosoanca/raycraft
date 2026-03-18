#include "renderer.h"
#include <algorithm>
#include "player.h"

namespace Renderer
{
	void render(render_vec& rv) {
		const auto [x, y, z] = Player::camera.position;
		World::WorldPos player_pos{static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(z)};
		std::sort(rv.rbegin(), rv.rend(),
			[player_pos](const render_pair& rp1, const render_pair& rp2) {
				return rp1.second-player_pos.get_chunk_pos() < rp2.second-player_pos.get_chunk_pos();
			}
		);
		for (auto& [chunk_model, chunk_pos] : rv) {
			chunk_model->render(chunk_pos);
		}
	}
}
