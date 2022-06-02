#include <Windows.h>

namespace wcry {

	namespace game {

		extern void init_external_interface();

		extern int java_to_cpp(int java);
		extern void get_minecraft_binds();

		namespace binds {

			inline int slot1 = 0x31; //1
			inline int slot2 = 0x32; //2
			inline int slot3 = 0x33; //3
			inline int slot4 = 0x34; //4
			inline int slot5 = 0x35; //5
			inline int slot6 = 0x36; //6
			inline int slot7 = 0x37; //7
			inline int slot8 = 0x38; //8
			inline int slot9 = 0x39; //9

			inline int w = 0x57; //w
			inline int a = 0x41; //a
			inline int s = 0x53; //s
			inline int d = 0x44; //d

			inline int jump = VK_SPACE;
			inline int sprint = VK_LCONTROL;
			inline int sneak = VK_LSHIFT;

			inline int drop = 0x51; //q
			inline int inventory = 0x45; //e

			inline float sensitivity = 0.5f;
		};

		namespace local_player {

			inline bool moving = false;
			inline bool jumping = false;
			inline bool sprinting = false;
			inline bool sneaking = false;

			inline bool in_inventory = false;
			inline bool clicking = false;

			inline int current_slot = 0;

		};
	}
}