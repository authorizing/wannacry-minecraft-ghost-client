#include "interface.h"
#include "assets/wrappers.h"
#include "../modules/modules.h"
#include "../user/thread.h"
#include "../helpers/config/configs.h"

#include <ShlObj.h>

HWND wcry::user_interface::window_hwnd = nullptr;
ImVec4 wcry::user_interface::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
int wcry::user_interface::main_tab = 0;

static const char* tabs_array[] = { "A", "B", "C" };
static const char* cmboat_tabs_array[] = { "Aim assist", "Autoclicker", "Reach", "Velocity" };
static const char* settings_tabs_array[] = { "Main", "Configs", "Themes", "Friends" };

std::vector <std::string> files;

namespace wcry {

    void user_interface::run()
    {
        static float loading_bar_status = 0.f;

        static int tablinepos = 90;
        static int i = 90;

        static int tablinepos_2, tablinepos_3, tablinepos_4 = 48;
        static int i_2[3] = {48, 48, 48};

        char buf[1024];

        auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("Edited", nullptr, flags);
        {
            ImVec2 p = ImGui::GetWindowPos();
            ImDrawList* draw = ImGui::GetWindowDrawList();

            if (wcry::session_status <= 4) {

                render_load_background();

                switch (wcry::session_status) {
                case -2:
                    render_text_center(wcry::session_return.c_str(), 240);
                    break;
                case -1:

                    wcry::_get_windows();

                    if (wcry::p_windows.empty()) {
                        render_text_center("Waiting for minecraft instance", 240);
                    }
                    else {
                        render_text_center("Select a minecraft instance", 240);
                    }

                    ImGui::SetCursorPos(ImVec2(
                        (ImGui::GetIO().DisplaySize.x - 220) / 2,
                        280)
                    );

                    for (auto window : wcry::p_windows) {

                        auto text_len = GetWindowTextLengthA(window) + 1;
                        auto window_buf = new char[text_len];
                        DWORD pid_buf;

                        GetWindowTextA(window,
                            window_buf,
                            text_len
                        );

                        GetWindowThreadProcessId(window,
                            &pid_buf
                        );

                        const char* text = window_buf;
                        IDirect3DTexture9* texture = minecraft_logo;
                        int warning = 0;


                        if (strstr(window_buf, "Lunar")) {
                            texture = lunar_logo;
                            text = "Lunar Client";
                            warning = 2;
                        }
                        else if (strstr(window_buf, "Badlion")) {
                            text = "Badlion Client";
                            warning = 1;
                        }
                        else if (strstr(window_buf, "Cheatbreaker")) {
                            text = "Cheatbreaker";
                            warning = 1;
                        }

                        sprintf_s(buf, "%s\nPID: %i", text, pid_buf);

                        ImGui::NewLine();
                        ImGui::SameLine((ImGui::GetIO().DisplaySize.x - 220) / 2);

                        if (image_button(texture, buf, { 30, 30 }, 5, 210, warning)) {
                            wcry::p_pid = pid_buf;
                            wcry::p_hwnd = window;
                        }

                    }

                    wcry::p_windows.clear();

                    break;

                case 1:
                    if (loading_bar_status <= 0.16f) { loading_bar_status += 0.01f; } break;
                case 2:
                    if (loading_bar_status <= 0.33f) { loading_bar_status += 0.01f; } break;
                case 3:
                    if (loading_bar_status <= 0.5) { loading_bar_status += 0.01f; } break;
                case 4:
                    if (loading_bar_status <= 0.66) { loading_bar_status += 0.01f; } break;
                case 5:
                    if (loading_bar_status <= 0.83) { loading_bar_status += 0.01f; } break;
                case 6:
                    if (loading_bar_status <= 1.f) { loading_bar_status += 0.01f; } break;
                }

                if (wcry::session_status >= 0) {
                    render_loading(loading_bar_status);
                }
            }
            else {

                render_client_background();
                render_user("kura");

                render_tab_line(tablinepos, i);

                render_tabs(main_tab, i);

                switch (main_tab) {
                    case 0:
                    {

                        static int combat_tab = 0;

                        render_subtab_line(tablinepos_2, i_2, 44);
                        render_sub_tabs(cmboat_tabs_array, combat_tab, i_2);

                        switch (combat_tab) {
                            case 0: {

                                begin_group_box("Aim Assist", ImVec2(178, 68), ImVec2(236, 390));

                                checkbox("Enabled", &aim::enabled);
                                slider_float("Field of view", &aim::max_angle, 20, 180, "%.1f", NULL);
                                slider_float("Speed", &aim::aim_speed, 5, 100, "%.1f", NULL);
                                slider_float("Minimum distance", &aim::aim_distance, 3, 8, "%.1f", NULL);

                                combo_box("Method", &aim::method, "Hitbox\0Multipoint");

                                if (aim::method == 0)
                                    combo_box("Hitbox", &aim::hitbox, "Head\0Body\0Legs\0Feet");

                                end_group_box();

                                begin_group_box("Flags", ImVec2(445, 68), ImVec2(236, 390));

                                checkbox("Only while clicking", &aim::on_click);
                                checkbox("Only while aiming", &aim::on_move);
                                checkbox("Vertical check", &aim::vertical);
                                checkbox("Antibot", &aim::antibot);

                                end_group_box();

                            } break;
                            case 1: {

                                begin_group_box("Auto clicker", ImVec2(178, 68), ImVec2(236, 390));

                                checkbox("Enabled", &aclicker::enabled);

                                keybind("Keybind##ac", &aclicker::key, { 180, 20 });

                                slider_float("Minimum CPS", &aclicker::cps_min, 2, 20, "%.1f", NULL);
                                slider_float("Maximum CPS", &aclicker::cps_max, 2, 20, "%.1f", NULL);

                                slider_float("Jitter horizontal", &aclicker::jitter_x, 0, 5, "%.2f", NULL);
                                slider_float("Jitter vertical", &aclicker::jitter_y, 0, 5, "%.2f", NULL);

                                slider_int("Blockhit delay", &aclicker::blockhit_delay, 200, 1000);

                                combo_box_wrapped("Breakblocks", &aclicker::breakblocks_mode, "Disabled\0Native\0RMB Lock\0Dynamic");

                                end_group_box();

                                begin_group_box("Flags", ImVec2(445, 68), ImVec2(236, 390));



                                end_group_box();

                            } break;
                            case 2: {

                                begin_group_box("Reach", ImVec2(178, 68), ImVec2(236, 390));

                                slider_float("Minimum distance", &reach::distance_min, 3, 6, "%.2f", NULL);
                                slider_float("Maximum distance", &reach::distance_max, 3, 6, "%.2f", NULL);
                                slider_float("Chance", &reach::chance, 1, 100, "%1.f", NULL);

                                end_group_box();

                                begin_group_box("Flags", ImVec2(445, 68), ImVec2(236, 390));

                                end_group_box();

                            } break;

                        } break;

                    } break;
                    case 2:
                    {
                        static int settings_tab = 0;

                        render_sub_tabs(settings_tabs_array, settings_tab, i_2);

                        switch (settings_tab) {
                        case 0: {
                            
                          

                        } break;
                        case 1: {

                            static int selected_config = 0;

                            begin_group_box("Configs", ImVec2(178, 68), ImVec2(236, 390));

                            static auto should_update = true;

                            if (should_update)
                            {
                                should_update = false;

                                cfg_manager->config_files();
                                files = cfg_manager->files;

                                for (auto& current : files)
                                    if (current.size() > 2)
                                        current.erase(current.size() - 3, 3);
                            }

                            if (button(xor ("Open folder"), ImVec2(150, 20)))
                            {
                                std::string folder;

                                auto get_dir = [&folder]() -> void
                                {
                                    static TCHAR path[MAX_PATH];

                                    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, NULL, path)))
                                        folder = std::string(path) + xor ("\\wannacry\\configs\\");

                                    CreateDirectory(folder.c_str(), NULL);
                                };

                                get_dir();
                                ShellExecuteA(NULL, xor ("open"), folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            }

                            static char config_name[64] = "\0";
                            ImGui::InputText(xor ("##configname"), config_name, sizeof(config_name));

                            if (button(xor ("Refresh configs")))
                            {
                                cfg_manager->config_files();
                                files = cfg_manager->files;

                                for (auto& current : files)
                                    if (current.size() > 2)
                                        current.erase(current.size() - 3, 3);
                            }

                            if (button(xor ("Create config")))
                            {
                                std::string new_config_name = config_name;

                                auto empty = true;

                                for (auto current : new_config_name)
                                {
                                    if (current != ' ')
                                    {
                                        empty = false;
                                        break;
                                    }
                                }

                                if (empty)
                                    new_config_name = xor ("config");

                                if (new_config_name.find(xor (".wc")) == std::string::npos)
                                    new_config_name += xor (".wc");

                                cfg_manager->save(new_config_name);
                                cfg_manager->config_files();

                                selected_config = cfg_manager->files.size() - 1;
                                files = cfg_manager->files;

                                for (auto& current : files)
                                    if (current.size() > 2)
                                        current.erase(current.size() - 3, 3);
                            }

                            static auto next_save = false;
                            static auto prenext_save = false;
                            static auto clicked_sure = false;
                            static auto save_time = ImGui::GetTime();
                            static auto save_alpha = 1.0f;

                            const auto clamp = [&](float in, float low, float high) {
                                if (in <= low)
                                    return low;

                                if (in >= high)
                                    return high;

                                return in;
                            };

                            save_alpha = clamp(save_alpha + (4.f * ImGui::GetIO().DeltaTime * (!prenext_save ? 1.f : -1.f)), 0.01f, 1.f);
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, save_alpha);

                            if (!next_save)
                            {
                                clicked_sure = false;

                                if (prenext_save && save_alpha <= 0.01f)
                                    next_save = true;

                                if (button(xor ("Save config")))
                                {
                                    save_time = ImGui::GetTime();
                                    prenext_save = true;
                                }
                            }
                            else
                            {
                                if (prenext_save && save_alpha <= 0.01f)
                                {
                                    prenext_save = false;
                                    next_save = !clicked_sure;
                                }

                                if (button(xor ("Are you sure?")))
                                {
                                    if (!cfg_manager->files.empty()) {

                                        cfg_manager->save(cfg_manager->files.at(selected_config));
                                        cfg_manager->config_files();

                                    }

                                    prenext_save = true;
                                    clicked_sure = true;
                                }

                                if (!clicked_sure && ImGui::GetTime() > save_time + 1.5f)
                                {
                                    prenext_save = true;
                                    clicked_sure = true;
                                }
                            }

                            ImGui::PopStyleVar();

                            if (button(xor ("Load config"))) {
                                if (!cfg_manager->files.empty()) {
                                    cfg_manager->load(cfg_manager->files.at(selected_config));
                                    cfg_manager->config_files();
                                }
                            }

                            static auto next_delete = false;
                            static auto prenext_delete = false;
                            static auto clicked_sure_del = false;
                            static auto delete_time = ImGui::GetTime();
                            static auto delete_alpha = 1.0f;

                            delete_alpha = clamp(delete_alpha + (4.f * ImGui::GetIO().DeltaTime * (!prenext_delete ? 1.f : -1.f)), 0.01f, 1.f);
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, delete_alpha);

                            if (!next_delete)
                            {
                                clicked_sure_del = false;

                                if (prenext_delete && delete_alpha <= 0.01f)
                                    next_delete = true;

                                if (button(xor ("Remove config")))
                                {
                                    delete_time = ImGui::GetTime();
                                    prenext_delete = true;
                                }
                            }
                            else
                            {
                                if (prenext_delete && delete_alpha <= 0.01f)
                                {
                                    prenext_delete = false;
                                    next_delete = !clicked_sure_del;
                                }

                                if (button(xor ("Are you sure?")))
                                {
                                    if (!cfg_manager->files.empty()) {
                                        cfg_manager->remove(cfg_manager->files.at(selected_config));
                                        cfg_manager->config_files();

                                        files = cfg_manager->files;

                                        if (selected_config >= files.size())
                                            selected_config = files.size() - 1;

                                        for (auto& current : files)
                                            if (current.size() > 2)
                                                current.erase(current.size() - 3, 3);
                                    }

                                    prenext_delete = true;
                                    clicked_sure_del = true;
                                }

                                if (!clicked_sure_del && ImGui::GetTime() > delete_time + 1.5f)
                                {
                                    prenext_delete = true;
                                    clicked_sure_del = true;
                                }
                            }

                            ImGui::PopStyleVar();

                            end_group_box();

                            begin_group_box("List", ImVec2(445, 68), ImVec2(236, 390));

                            if (ImGui::ListBoxHeader(xor ("##cfg_list"), ImVec2(200, 330)))
                            {
                                for (int i = 0; i < files.size(); i++)
                                {
                                    if (ImGui::Selectable(files[i].c_str(), i == selected_config))
                                        selected_config = i;
                                }
                                ImGui::ListBoxFooter();
                            }

                            end_group_box();

                        } break;
                        case 2: {

                            begin_group_box("Reach", ImVec2(178, 68), ImVec2(236, 390));

                            slider_float("Minimum distance", &reach::distance_min, 3, 6, "%.2f", NULL);
                            slider_float("Maximum distance", &reach::distance_max, 3, 6, "%.2f", NULL);
                            slider_float("Chance", &reach::chance, 1, 100, "%1.f", NULL);

                            end_group_box();

                            begin_group_box("Flags", ImVec2(445, 68), ImVec2(236, 390));

                            end_group_box();

                        } break;

                        } break;
                    } break;
                }
            }
        }
        ImGui::End();
    }

    void user_interface::setup_styles(ImGuiStyle& style)
    {
        style.WindowPadding = ImVec2(0, 0);
        style.FramePadding = ImVec2(4, 3);
        style.ItemSpacing = ImVec2(8, 10);
        style.ItemInnerSpacing = ImVec2(4, 4);
        style.IndentSpacing = 21;
        style.ScrollbarSize = 1;
        style.GrabMinSize = 1;
        style.WindowBorderSize = 0;
        style.ChildBorderSize = 0;
        style.PopupBorderSize = 1;
        style.FrameBorderSize = 1;
        style.WindowRounding = 0;
        style.ChildRounding = 5;
        style.FrameRounding = 5;
        style.PopupRounding = 0;
        style.ScrollbarRounding = 0;
        style.GrabRounding = 0;
        style.SelectableTextAlign = ImVec2(0.5f, 0.5f);

        style.Colors[ImGuiCol_Text] = ImVec4(0.900000f, 0.900000f, 0.900000f, 1.000000f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.600000f, 0.600000f, 0.600000f, 1.000000f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.129412f, 0.141176f, 0.168627f, 1.000000f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.176471f, 0.196078f, 0.219608f, 1.000000f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.110000f, 0.110000f, 0.140000f, 0.920000f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.500000f, 0.500000f, 0.500000f, 0.500000f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.000000f, 0.000000f, 0.000000f, 0.000000f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.430000f, 0.430000f, 0.430000f, 0.390000f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.470000f, 0.470000f, 0.690000f, 0.400000f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.420000f, 0.410000f, 0.640000f, 0.690000f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.270000f, 0.270000f, 0.540000f, 0.830000f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.320000f, 0.320000f, 0.630000f, 0.870000f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.400000f, 0.400000f, 0.800000f, 0.200000f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.400000f, 0.400000f, 0.550000f, 0.800000f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.200000f, 0.250000f, 0.300000f, 0.600000f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.400000f, 0.400000f, 0.800000f, 0.300000f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.400000f, 0.400000f, 0.800000f, 0.400000f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.410000f, 0.390000f, 0.800000f, 0.600000f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.900000f, 0.900000f, 0.900000f, 0.500000f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.000000f, 1.000000f, 1.000000f, 0.300000f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.410000f, 0.390000f, 0.800000f, 0.600000f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.129412f, 0.141176f, 0.168627f, 1.000000f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.109412f, 0.111176f, 0.128627f, 1.00000f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.109412f, 0.111176f, 0.128627f, 1.00000f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.400000f, 0.400000f, 0.900000f, 0.450000f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.450000f, 0.450000f, 0.900000f, 0.800000f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.530000f, 0.530000f, 0.870000f, 0.800000f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.500000f, 0.500000f, 0.500000f, 0.600000f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.600000f, 0.600000f, 0.700000f, 1.000000f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.700000f, 0.700000f, 0.900000f, 1.000000f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.000000f, 1.000000f, 1.000000f, 0.160000f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.780000f, 0.820000f, 1.000000f, 0.600000f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.780000f, 0.820000f, 1.000000f, 0.900000f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.000000f, 0.000000f, 1.000000f, 0.350000f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.000000f, 1.000000f, 0.000000f, 0.900000f);
    }

}