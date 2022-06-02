
#include "../../game/game.h"

namespace wcry {

	namespace scanner {

		namespace addresses {

			//memory stuff, for use on lunar client, before mappings can be updated
			//will be fixed soon :D

			namespace reach {
				extern std::vector<size_t> unlimit;
				extern std::vector<size_t> floats;
				extern std::vector<size_t> doubles;
			}

			namespace glide {
				extern std::vector<size_t> locationsglide;
			}

			namespace nametags {
				extern std::vector<size_t> locations1;
				extern std::vector<size_t> locations2;
			}

			namespace esp {
				extern std::vector<size_t> locations;
			}

			namespace cavefinder {
				extern std::vector<size_t> locations;
			}

			namespace timer {
				extern std::vector<size_t> locations;
			}

			namespace speed {
				extern std::vector<size_t> locations;
			}

			namespace bhop {
				extern std::vector<size_t> locations;
			}
		}

		extern size_t pattern(size_t min, size_t max, std::vector<int> pattern);
		extern void get_values();
	}
}