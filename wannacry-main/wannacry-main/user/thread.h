#include <Windows.h>
#include <string>
#include <mutex>

#include "../sdk/scanner/scanner.h"
#include "../game/game.h"

namespace wcry {

	inline int session_status = 0;
	inline std::string session_return = "";

	inline void initilise_user_thread() {

		const auto user_thread = [&]() {

			std::once_flag flag;
			
			//initialize server connection
			session_status = 1;
			
			Sleep(1000);
			session_status = 2;

#ifdef _DEBUG
			goto debug;
#endif
			//authorize user
			Sleep(1000);
			session_status = 3;

			while (wcry::p_pid == 0) {
				session_status = -1;
				Sleep(1000);
			}

			if (!wcry::_open_mc(wcry::p_pid)) {
				
				session_status = -2;
				session_return = "Error opening minecraft process";

				return;
			}

			session_status = 4;
			wcry::_get_ver();
			Sleep(100);

			while (!wcry::mc_version || wcry::mc_is_lunr_new) {

				session_status = -2;
				session_return = "This minecraft version is not supported";

				Sleep(1000);
			}

			if (wcry::mc_version && !wcry::mc_is_lunr) {

				wcry::mc_c_ptr = wcry::scanner::pattern(0x100000000, 0x1ffffffff, {

				   RAND, 0x01, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, RAND, 0x00, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, 0x00, 0x01, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
				   0x28, 0x0F, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, 0x00, 0x01, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, RAND, 0x00, 0x00, 0x00, 0x00,
				   0x28, 0x0F, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, RAND, 0x01, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, RAND, 0x01, 0x00, 0x00, 0x00,
				   RAND, RAND, RAND, RAND, RAND, 0x00, 0x00, 0x00,
				   0x01, RAND, RAND, RAND, RAND, RAND, RAND, 0x01,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, RAND, RAND, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00

				}) + 0x60;

				if (wcry::mc_c_ptr <= 0x100) {
					printf("error from bytes");
					session_status = -2;
					session_return = "This minecraft version is not supported";
					while (true) {
						Sleep(1000);
					}
				}
			}

			//launch threads
			Sleep(1000);
			session_status = 5;

			wcry::_thread(wcry::_handler);

		debug:

			//done
			Sleep(1000);
			session_status = 6;

			//while logged in, check connection
		};

		std::thread user_session_thread(
			user_thread
		);

		user_session_thread.detach();

		return;
	}


}
