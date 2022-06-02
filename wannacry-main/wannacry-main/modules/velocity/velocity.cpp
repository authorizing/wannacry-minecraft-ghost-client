
#include "../modules.h"

void velocity::handler() {

	for (static bool motion_set; "velocity"; Sleep(1)) {

		if (!wcry::minecraft->address || !wcry::current_world->address ||
			!wcry::local_player->address) continue;

		vec3 motion = wcry::local_player->get_motion();
		static vec3 _motion_ = { 0.0, 0.0, 0.0 };

		if (motion.y >= 9. || motion.y <= -9 || isnan(motion.y)) continue;

		if (!motion_set && wcry::local_player->get_hurt_timer() >= 19) {

			if (
				_motion_.x != motion.x ||
				_motion_.y != motion.y ||
				_motion_.z != motion.z
				) {

				wcry::local_player->set_motion({
					motion.x * (horizontal / 100.),
					motion.y * (vertical / 100.),
					motion.z * (horizontal / 100.)
				});

				motion_set = true;

			}

		} else 
			if (motion_set && wcry::local_player->get_hurt_timer() < 19)
				motion_set = false; _motion_ = motion;

	}

}
