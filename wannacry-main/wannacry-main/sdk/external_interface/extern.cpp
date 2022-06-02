#include "extern.h"

#include "../../game/game.h"
#include "../class_list/minecraft/minecraft.h"

#include <fstream>

void movement_detect();
void inventory_detect();
void bind_detect();

void wcry::game::init_external_interface() {
    
    wcry::_thread(bind_detect);
    wcry::_thread(inventory_detect);
    wcry::_thread(movement_detect);
}

void bind_detect() {
    while (true) {
        wcry::game::get_minecraft_binds();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void movement_detect() {

    while (true) {

        if (wcry::_is_foreground_window(wcry::p_hwnd) && 
            !wcry::minecraft->is_in_menu()) {

            if (wcry::_is_pressed(wcry::game::binds::w)) {
                wcry::game::local_player::moving = true;
            }
            else {
                if (wcry::_is_pressed(wcry::game::binds::a)) {
                    wcry::game::local_player::moving = true;
                }
                else {
                    if (wcry::_is_pressed(wcry::game::binds::s)) {
                        wcry::game::local_player::moving = true;
                    }
                    else {
                        if (wcry::_is_pressed(wcry::game::binds::d)) {
                            wcry::game::local_player::moving = true;
                        }
                        else {
                            wcry::game::local_player::moving = false;
                        }
                    }
                }
            }

            if (wcry::_is_pressed(wcry::game::binds::jump)) {
                wcry::game::local_player::jumping = true;
            }
            else {
                wcry::game::local_player::jumping = false;
            }

            if (wcry::_is_pressed(wcry::game::binds::sneak)) {
                wcry::game::local_player::sneaking = true;
            }
            else {
                wcry::game::local_player::sneaking = false;
            }

            if (wcry::_is_pressed(wcry::game::binds::sprint)) {
                if (wcry::game::local_player::moving) {
                    wcry::game::local_player::sprinting = true;
                }
                else {
                    wcry::game::local_player::sprinting = false;
                }
            }
            else {
                wcry::game::local_player::sprinting = false;
            }

        }
        else {
            wcry::game::local_player::sprinting = false;
            wcry::game::local_player::jumping = false;
            wcry::game::local_player::moving = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void inventory_detect() {
    while (true) {

        if (!wcry::minecraft->is_in_menu()) {

            if (wcry::_is_pressed(wcry::game::binds::slot1)) {
                wcry::game::local_player::current_slot = 1;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot2)) {
                wcry::game::local_player::current_slot = 2;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot3)) {
                wcry::game::local_player::current_slot = 3;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot4)) {
                wcry::game::local_player::current_slot = 4;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot5)) {
                wcry::game::local_player::current_slot = 5;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot6)) {
                wcry::game::local_player::current_slot = 6;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot7)) {
                wcry::game::local_player::current_slot = 7;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot8)) {
                wcry::game::local_player::current_slot = 8;
            }

            if (wcry::_is_pressed(wcry::game::binds::slot9)) {
                wcry::game::local_player::current_slot = 9;
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
}

void wcry::game::get_minecraft_binds() {  

    std::ifstream bindsfile(getenv(xor("APPDATA")) + std::string(xor("\\.minecraft\\options.txt")));
    std::string line;

    while (std::getline(bindsfile, line)) {
        if (strstr(line.c_str(), xor("key_key.hotbar.1"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot1 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.2"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot2 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.3"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot3 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.4"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot4 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.5"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot5 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.6"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot6 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.7"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot7 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.8"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot8 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.hotbar.9"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::slot9 = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }

        if (strstr(line.c_str(), xor("key_key.forward"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::w = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.back"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::s = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.left"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::a = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.right"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::d = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.jump"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::jump = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.sprint"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::sprint = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.drop"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::drop = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.inventory"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::inventory = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
        if (strstr(line.c_str(), xor("key_key.sneak"))) {
            if (line.substr(line.rfind(":") + 1).length() < 4) {
                wcry::game::binds::sneak = java_to_cpp(stoi(line.substr(line.rfind(":") + 1)));
            }
        }
    }
}

int wcry::game::java_to_cpp(int minecraft) {
    if (minecraft == 0) { return 0; }
    if (minecraft == 1) { return VK_ESCAPE; }
    if (minecraft == 2) { return 0x31; }
    if (minecraft == 3) { return 0x32; }
    if (minecraft == 4) { return 0x33; }
    if (minecraft == 5) { return 0x34; }
    if (minecraft == 6) { return 0x35; }
    if (minecraft == 7) { return 0x36; }
    if (minecraft == 8) { return 0x37; }
    if (minecraft == 9) { return 0x38; }
    if (minecraft == 10) { return 0x39; }
    if (minecraft == 11) { return VK_NUMPAD0; }
    if (minecraft == 12) { return VK_SUBTRACT; }
    if (minecraft == 14) { return VK_BACK; }
    if (minecraft == 15) { return VK_TAB; }
    if (minecraft == 16) { return 'Q'; }
    if (minecraft == 17) { return 'W'; }
    if (minecraft == 18) { return 'E'; }
    if (minecraft == 19) { return 'R'; }
    if (minecraft == 20) { return 'T'; }
    if (minecraft == 21) { return 'Y'; }
    if (minecraft == 22) { return 'U'; }
    if (minecraft == 23) { return 'I'; }
    if (minecraft == 24) { return 'O'; }
    if (minecraft == 25) { return 'P'; }
    if (minecraft == 28) { return VK_RETURN; }
    if (minecraft == 29) { return VK_CONTROL; }
    if (minecraft == 30) { return 'A'; }
    if (minecraft == 31) { return 'S'; }
    if (minecraft == 32) { return 'D'; }
    if (minecraft == 33) { return 'F'; }
    if (minecraft == 34) { return 'G'; }
    if (minecraft == 35) { return 'H'; }
    if (minecraft == 36) { return 'J'; }
    if (minecraft == 37) { return 'K'; }
    if (minecraft == 38) { return 'L'; }
    if (minecraft == 42) { return VK_LSHIFT; }
    if (minecraft == 44) { return 'Z'; }
    if (minecraft == 45) { return 'X'; }
    if (minecraft == 46) { return 'C'; }
    if (minecraft == 47) { return 'V'; }
    if (minecraft == 48) { return 'B'; }
    if (minecraft == 49) { return 'N'; }
    if (minecraft == 50) { return 'M'; }
    if (minecraft == 54) { return VK_RSHIFT; }
    if (minecraft == 55) { return VK_MULTIPLY; }
    if (minecraft == 56) { return VK_LMENU; }
    if (minecraft == 57) { return VK_SPACE; }
    if (minecraft == 58) { return VK_CAPITAL; }
    if (minecraft == 59) { return VK_F1; }
    if (minecraft == 60) { return VK_F2; }
    if (minecraft == 61) { return VK_F3; }
    if (minecraft == 62) { return VK_F4; }
    if (minecraft == 63) { return VK_F5; }
    if (minecraft == 64) { return VK_F6; }
    if (minecraft == 65) { return VK_F7; }
    if (minecraft == 66) { return VK_F8; }
    if (minecraft == 67) { return VK_F9; }
    if (minecraft == 68) { return VK_F10; }
    if (minecraft == 69) { return VK_NUMLOCK; }
    if (minecraft == 70) { return VK_SCROLL; }
    if (minecraft == 71) { return VK_NUMPAD7; }
    if (minecraft == 72) { return VK_NUMPAD8; }
    if (minecraft == 73) { return VK_NUMPAD9; }
    if (minecraft == 74) { return VK_SUBTRACT; }
    if (minecraft == 75) { return VK_NUMPAD4; }
    if (minecraft == 76) { return VK_NUMPAD5; }
    if (minecraft == 77) { return VK_NUMPAD6; }
    if (minecraft == 78) { return VK_ADD; }
    if (minecraft == 79) { return VK_NUMPAD1; }
    if (minecraft == 80) { return VK_NUMPAD2; }
    if (minecraft == 81) { return VK_NUMPAD3; }
    if (minecraft == 82) { return VK_NUMPAD0; }
    if (minecraft == 83) { return VK_DECIMAL; }
    if (minecraft == 87) { return VK_F11; }
    if (minecraft == 88) { return VK_F12; }
    if (minecraft == 100) { return VK_F13; }
    if (minecraft == 101) { return VK_F14; }
    if (minecraft == 102) { return VK_F15; }
    if (minecraft == 103) { return VK_F16; }
    if (minecraft == 104) { return VK_F17; }
    if (minecraft == 105) { return VK_F18; }
    if (minecraft == 112) { return VK_KANA; }
    if (minecraft == 113) { return VK_F19; }
    if (minecraft == 121) { return VK_CONVERT; }
    if (minecraft == 123) { return VK_NONCONVERT; }
    if (minecraft == 157) { return VK_RCONTROL; }
    if (minecraft == 179) { return VK_OEM_COMMA; }
    if (minecraft == 181) { return VK_DIVIDE; }
    if (minecraft == 184) { return VK_RMENU; }
    if (minecraft == 197) { return VK_PAUSE; }
    if (minecraft == 199) { return VK_HOME; }
    if (minecraft == 200) { return VK_UP; }
    if (minecraft == 201) { return VK_PRIOR; }
    if (minecraft == 203) { return VK_LEFT; }
    if (minecraft == 205) { return VK_RIGHT; }
    if (minecraft == 207) { return VK_END; }
    if (minecraft == 208) { return VK_DOWN; }
    if (minecraft == 209) { return VK_NEXT; }
    if (minecraft == 210) { return VK_INSERT; }
    if (minecraft == 211) { return VK_DELETE; }
    if (minecraft == -100) { return 0x01; }
    if (minecraft == -99) { return 0x02; }
    if (minecraft == -98) { return 0x04; }
    if (minecraft == -97) { return 0x05; }
    if (minecraft == -96) { return 0x06; }
    if (minecraft == -95) { return 0x06; }
    if (minecraft == -94) { return 0x06; }
    if (minecraft == -93) { return 0x06; }
    if (minecraft == -92) { return 0x06; }
    if (minecraft == -91) { return 0x06; }
    if (minecraft == -90) { return 0x06; }
    if (minecraft == -89) { return 0x06; }
    if (minecraft == -88) { return 0x06; }
    if (minecraft == -87) { return 0x06; }
    if (minecraft == -86) { return 0x06; }
    if (minecraft == -85) { return 0x06; }
}