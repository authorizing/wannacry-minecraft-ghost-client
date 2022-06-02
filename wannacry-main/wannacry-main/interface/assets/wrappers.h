#include "assets.h"

#define render_load_background() \
    auto image_dimensions = ImVec2(120, 120); /*image dimensions*/ \
    auto image_width = (ImGui::GetIO().DisplaySize.x - image_dimensions.x) / 2, image_height = (ImGui::GetIO().DisplaySize.y - image_dimensions.y) / 4; \
    draw->AddRectFilled(ImVec2(p. x + 0, p.y + 0), ImVec2(p.x + ImGui::GetIO().DisplaySize.x, p.y + ImGui::GetIO().DisplaySize.y), ImColor(45, 50, 56, 255), 0, 5); \
    draw->AddImage(large_logo, ImVec2(p.x + image_width, p.y + image_height), ImVec2(p.x + image_dimensions.x + image_width , p.y + image_dimensions.y + image_height)); \

#define render_text_pos(text, pos) \
    draw->AddText(pos, ImGui::GetColorU32(ImGuiCol_Text), text); \

#define render_text_center(text, y_offset) \
    draw->AddText(ImVec2((ImGui::GetIO().DisplaySize.x - ImGui::CalcTextSize(text).x) / 2, y_offset), ImGui::GetColorU32(ImGuiCol_Text), text); \

#define render_loading(status) \
    auto loading_bar_dimensions = ImVec2(210, 8); \
    auto image_offset = (ImGui::GetIO().DisplaySize.y - image_dimensions.y) / 4; \
    buffering_bar("loading_bar", (float)status, \
        ImVec2(p.x + (ImGui::GetIO().DisplaySize.x - loading_bar_dimensions.x) / 2, \
        p.y + image_dimensions.y + image_offset + 50), \
    loading_bar_dimensions, \
    ImVec4(0.129412f, 0.141176f, 0.168627f, 1.000000f), \
    ImVec4(33 / 255.f, 150 / 255.f, 243 / 255.f, 255 / 255.f) \
    ); \

#define render_client_background() \
    draw->AddRectFilled(ImVec2(p.x + 0, p.y + 0), ImVec2(p.x + 44, p.y + ImGui::GetIO().DisplaySize.y), ImColor(45, 50, 56, 255), 0, 5); \
    draw->AddRectFilled(ImVec2(p.x + 44, p.y + 0), ImVec2(p.x + 148, p.y + ImGui::GetIO().DisplaySize.y), ImColor(45, 50, 56, 255), 0, 5); \
    draw->AddRectFilled(ImVec2(p.x + 44, p.y + 0), ImVec2(p.x + ImGui::GetIO().DisplaySize.x, p.y + 38), ImColor(45, 50, 56, 255), 0, 2); \
    draw->AddRectFilledMultiColor(ImVec2(p.x + 44, p.y + 0), ImVec2(p.x + 56, p.y + ImGui::GetIO().DisplaySize.y), ImColor(9, 9, 10, 58), ImColor(9, 9, 10, 0), ImColor(9, 9, 10, 0), ImColor(9, 9, 10, 58)); \
    draw->AddRectFilledMultiColor(ImVec2(p.x + 44, p.y + 38), ImVec2(p.x + ImGui::GetIO().DisplaySize.x, p.y + 50), ImColor(9, 9, 10, 58), ImColor(9, 9, 10, 58), ImColor(9, 9, 10, 0), ImColor(9, 9, 10, 0)); \
    draw->AddImage(small_logo, ImVec2(p.x + 10, p.y + 10), ImVec2(p.x + 34, p.y + 34)); \

#define render_user(user) \
	ImGui::PushFont(icons_small); \
    draw->AddText(ImVec2(p.x + ImGui::GetIO().DisplaySize.x - 45, p.y + 38 / 2 - ImGui::CalcTextSize("D").y / 2 + 1), ImColor(98 / 255.f, 101 / 255.f, 106 / 255.f, 255 / 255.f), "D"); \
    ImGui::PopFont(); \
    draw->AddText(ImVec2(p.x + ImGui::GetIO().DisplaySize.x - 45 - 10 - ImGui::CalcTextSize(user).x, p.y + 38 / 2 - ImGui::CalcTextSize(user).y / 2), ImColor(98 / 255.f, 101 / 255.f, 106 / 255.f, 255 / 255.f), user); \

#define render_tab_line(pos, index) \
    draw->AddLine(ImVec2(p.x, p.y + pos), ImVec2(p.x, p.y + pos + 32), ImColor(33, 150, 243), 2); \
    if (pos < index) \
     	pos += 5; \
	if (pos > index) \
		pos -= 5; \

#define render_subtab_line(pos, index, offset) \
    draw->AddLine(ImVec2(p.x + offset, p.y + pos), ImVec2(p.x + offset, p.y + pos + 28), ImColor(33, 150, 243), 2); \
    if (pos < index) \
     	pos += 5; \
	if (pos > index) \
		pos -= 5; \

template<size_t N>
inline void render_tabs_func(const char* (&names)[N], int& activetab, int& i_from_main, bool icon = true)
{
    bool values[N] = { false };

    values[activetab] = true;

    for(auto i = 0; i < N; ++i) {
        if (icon) {
            if (wcry::user_interface::tab(names[i], values[i])) {
                i_from_main = 90 + (i * 45);
                activetab = i;
            }
        }
        else {
            if (wcry::user_interface::sub_tab(names[i], values[i])) {
                i_from_main = 48 + (i * 45);
                activetab = i;
            }
        }
    }
}

#define render_tabs(current_tab, offset) \
	ImGui::SetCursorPos({ (float)0, (float)90 }); \
	ImGui::BeginGroup(); \
	render_tabs_func(tabs_array, current_tab, offset); \
	ImGui::EndGroup(); \


#define render_sub_tabs(tab_array, current_tab, offset) \
	ImGui::SetCursorPos({ (float)44, (float)38 }); \
	ImGui::BeginGroup(); \
	render_tabs_func(tab_array, current_tab, offset, false); \
    ImGui::EndGroup(); \

#define begin_group_box(id, location, size) \
    ImGui::SetCursorPos(location); \
    begin_child(id, size, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); \
    ImGui::SetCursorPos({ (float)15, (float)30 }); \
    begin_child(std::string(std::string(id) + "##1").c_str(), { size.x - 10, size.y - 50 }, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground); \
    ImGui::SetCursorPos(ImVec2(5, 5)); \
    ImGui::BeginGroup(); \


#define end_group_box() \
    ImGui::EndGroup(); \
    ImGui::EndChild(); \
    ImGui::EndChild(); \

#define combo_box_wrapped(label, ptr, items) \
    ImGui::Text(label); \
    combo_box(std::string(std::string("##") + label).c_str(), ptr, items);