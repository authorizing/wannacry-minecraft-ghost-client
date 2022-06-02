
#include "game.h"
#include "../modules/modules.h"
#include "../sdk/scanner/scanner.h"
#include "../helpers/config/configs.h"

#include <ShlObj.h>

HANDLE wcry::p_handle = NULL;
DWORD wcry::p_pid = NULL;
HWND wcry::p_hwnd = NULL;

std::vector<HWND> wcry::p_windows;

byte wcry::mc_version = NULL;
size_t wcry::mc_c_ptr = NULL;

bool wcry::mc_is_bdln = NULL;
bool wcry::mc_is_cbrk = NULL;
bool wcry::mc_is_lunr = NULL;
bool wcry::mc_is_lunr_new = NULL;

float wcry::mc_msense = 0.5f;

bool wcry::_open_mc(DWORD pid) {
    
    HANDLE* proc_hdl = &wcry::p_handle;

    if (!wcry::_get_process_handle(pid, proc_hdl)) {
        return false;
    }

    return true;

}

void wcry::_get_ver() {

    for (size_t adrs = 0x0; adrs < 0xffffffff; adrs += 100000) {

        std::string memory; memory.resize(100000);

        if (!NT_SUCCESS(ZwReadVirtualMemory(wcry::p_handle, (LPVOID)adrs, &memory[0], 100000, 0))) 
            continue;
    
        if (memory.find("Badlion") != std::string::npos) wcry::mc_is_bdln = true;
        if (memory.find("CheatBreaker") != std::string::npos) wcry::mc_is_cbrk = true;
        if (memory.find("Lunar") != std::string::npos) wcry::mc_is_lunr = true;
        if (memory.find("openjdk 16") != std::string::npos) wcry::mc_is_lunr_new = true;

        if (!wcry::mc_version) {

            //1.8.99 is present in 1.17, simple fix to prevent loading

            if (memory.find("1.8.9") != std::string::npos && 
                memory.find("1.8.99") == std::string::npos
               ) wcry::mc_version = V_189;

            if (memory.find("1.7.10") != std::string::npos) wcry::mc_version = V_1710;

        } else break;

    }

}

BOOL CALLBACK enum_windows_callback(HWND hWnd, LPARAM lParam) {

    char buf[255];
    RealGetWindowClassA(hWnd, buf, 255);

    if (std::string(buf) == "LWJGL") {
        wcry::p_windows.emplace_back(hWnd);
    }

    return true;
}

bool wcry::_get_windows() {
    return EnumWindows(enum_windows_callback, 0);
}

void wcry::_handler() {

    char path[MAX_PATH];
    std::string folder;

    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, NULL, path);
    folder = std::string(path) + xor("\\wannacry\\");

    CreateDirectoryA(folder.c_str(), 0);

    cfg_manager->setup();

    ULONG current_resolution;
    ZwSetTimerResolution(1, true, &current_resolution);

    if (!mc_is_lunr) {

        wcry::minecraft = std::make_shared<__minecraft>();
        wcry::current_world = std::make_shared<__world>();
        wcry::local_player = std::make_shared<__player>();

        wcry::_thread(aim::handler);
        wcry::_thread(velocity::handler);
    }

    wcry::_thread(aclicker::handler);

    while (true) {

        if (!mc_is_lunr) {
            if (minecraft->_get_obj()) {
                minecraft->_get_data();
            }
        }

        scanner::get_values();

        Sleep(50);
    }

}