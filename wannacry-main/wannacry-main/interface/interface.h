#include <d3d9.h>
#include <Windows.h>
#include <cstdlib>
#include <intrin.h>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"

namespace wcry {

	namespace user_interface {

		//indexes
		extern int main_tab;

		//globals
		extern WNDCLASSEX wc;
		extern HWND window_hwnd;
		extern ImVec4 clear_color;

		//elements
		extern IDirect3DTexture9* small_logo;
		extern IDirect3DTexture9* large_logo;

		extern IDirect3DTexture9* minecraft_logo;
		extern IDirect3DTexture9* lunar_logo;

		extern ImFont* montherat;
		extern ImFont* montherat_medium;
		extern ImFont* icons;
		extern ImFont* icons_small;

		//rendering
		extern void setup_styles(ImGuiStyle& style);
		extern void run();

		//windows
		extern void initialize();
		extern bool render();
		extern void shutdown();
	}

}