
#include "../game/game.h"
#include "../sdk/class_list/minecraft/minecraft.h"

namespace aclicker {

	extern void handler();

	extern bool enabled;

	extern int key;

	extern float cps_min;
	extern float cps_max;

	extern float jitter_x;
	extern float jitter_y;

	extern int blockhit_delay;

	extern int breakblocks_mode;
}

namespace aim {

	extern void handler();

	extern bool enabled;

	extern float max_angle;
	extern float aim_speed;
	extern float aim_distance;

	extern int method;
	extern int hitbox;

	extern bool on_click;
	extern bool on_move;
	extern bool vertical;
	extern bool antibot;

}

namespace reach {

	extern void handler();

	extern float distance_min;
	extern float distance_max;
	extern float chance;

}

namespace velocity {

	extern void handler();

	extern float horizontal;
	extern float vertical;
	extern float chance;
	extern float delay;

}