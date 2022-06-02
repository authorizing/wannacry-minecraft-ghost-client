
#include "game/game.h"

#include "interface/interface.h"
#include "user/thread.h"

int main() {

    wcry::initilise_user_thread();

    wcry::user_interface::initialize();

    if (!wcry::user_interface::render())
        wcry::user_interface::shutdown();

    return 0;

}
