
#include "world.h"

std::shared_ptr<__world> wcry::current_world = NULL;

void __world::set_address(size_t address) {

    this->address = address;

}

std::vector<size_t> __world::get_player_list() {

    std::vector<size_t> player_list;

    VAR_TYPE list;
    size_t list_field = NULL;

    switch (wcry::mc_version)
    {

    case V_189:
        list_field = this->address + 92;
        if (wcry::mc_is_bdln) list_field = this->address + 88;
        break;

    case V_1710:
        list_field = this->address + 84;
        break;

    }

    ZwReadVirtualMemory(wcry::p_handle, (LPVOID)list_field, &list, sizeof(VAR_TYPE), 0);

    std::cout << "0x" << std::hex << list << std::endl;

    VAR_TYPE list_size = 0, field_list_size = list + 16;

    ZwReadVirtualMemory(wcry::p_handle, (LPVOID)(size_t)field_list_size, &list_size, sizeof(VAR_TYPE), 0);

    list_size -= 1;

    VAR_TYPE local_plyr = 0, field_local_plyr = list + 40;

    ZwReadVirtualMemory(wcry::p_handle, (LPVOID)(size_t)field_local_plyr, &local_plyr, sizeof(VAR_TYPE), 0);

    if (local_plyr != wcry::local_player->address) {

        VAR_TYPE new_address = list + 20;
        ZwReadVirtualMemory(wcry::p_handle, (LPVOID)(size_t)new_address, &list, sizeof(VAR_TYPE), 0);
        list += 20;

    } else list += 44;

    if (list_size >= 100 || !list_size) return {};

    std::vector<VAR_TYPE> plyr_list(list_size);
    ZwReadVirtualMemory(wcry::p_handle, (LPVOID)(size_t)list, &plyr_list[0], (list_size * sizeof(VAR_TYPE)), 0);

    for (size_t it = 0; it < list_size; list += sizeof(VAR_TYPE), ++it) {

        player_list.push_back((size_t)plyr_list[it]);

    }

    return player_list;

}