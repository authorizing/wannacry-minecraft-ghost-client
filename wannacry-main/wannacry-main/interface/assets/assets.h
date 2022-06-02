#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include <map>

using namespace ImGui;

inline void shadow(ImDrawList* drawlist, ImVec2 pos, ImVec2 size, ImVec4 color, int rounding)
{
	while (true)
	{
		if (color.w <= 0.019f)
			break;

		drawlist->AddRect(pos, pos + size, ImGui::GetColorU32(color), rounding);
		color.w -= color.w / 12;
		pos -= ImVec2(1.f, 1.f);
		size += ImVec2(2.f, 2.f);
	}
}

namespace wcry {
	namespace user_interface
	{
		//CHILD
		bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
		{
			ImGuiContext& g = *GImGui;
			ImGuiWindow* parent_window = g.CurrentWindow;

			flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
			flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);

			const ImVec2 content_avail = ImGui::GetContentRegionAvail();
			ImVec2 size = ImFloor(size_arg);
			const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
			if (size.x <= 0.0f)
				size.x = ImMax(content_avail.x + size.x, 4.0f);
			if (size.y <= 0.0f)
				size.y = ImMax(content_avail.y + size.y, 4.0f);
			ImGui::SetNextWindowSize(size);

			char title[256];
			if (name)
				ImFormatString(title, IM_ARRAYSIZE(title), "%s/%s_%08X", parent_window->Name, name, id);
			else
				ImFormatString(title, IM_ARRAYSIZE(title), "%s/%08X", parent_window->Name, id);

			const float backup_border_size = g.Style.ChildBorderSize;
			if (!border)
				g.Style.ChildBorderSize = 0.0f;
			bool ret = ImGui::Begin(title, NULL, flags);
			g.Style.ChildBorderSize = backup_border_size;

			ImGuiWindow* child_window = g.CurrentWindow;
			child_window->ChildId = id;
			child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

			if (child_window->BeginCount == 1)
				parent_window->DC.CursorPos = child_window->Pos;

			if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayerActiveMask != 0 || child_window->DC.NavHasScroll))
			{
				ImGui::FocusWindow(child_window);
				ImGui::NavInitWindow(child_window, false);
				ImGui::SetActiveID(id + 1, child_window);
				g.ActiveIdSource = ImGuiInputSource_Nav;
			}

			if (!(flags & ImGuiWindowFlags_NoBackground))
			{
				shadow(ImGui::GetOverlayDrawList(), child_window->DC.CursorPos, size, ImVec4(9 / 255.f, 9 / 255.f, 10 / 255.f, 55 / 255.f), g.Style.ChildRounding);
				ImGui::RenderText(ImVec2(child_window->DC.CursorPos.x + 15, child_window->DC.CursorPos.y + 10), name);
			}

			return ret;
		}

		bool begin_child(const char* str_id, const ImVec2& size_arg, ImGuiWindowFlags extra_flags)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			return BeginChildEx(str_id, window->GetID(str_id), size_arg, false, extra_flags);
		}

		//CHECKBOX
		bool checkbox(const char* label, bool* v)
		{
			ImGuiWindow* window = GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
			const float square_sz = ImGui::GetFrameHeight();
			const ImVec2 pos = window->DC.CursorPos;
			const ImRect total_bb(pos, pos + ImVec2(35 + label_size.x, 12));
			ItemSize(total_bb, style.FramePadding.y);
			ItemAdd(total_bb, id);

			bool hovered, held;
			bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

			if (hovered || held)
				ImGui::SetMouseCursor(0);

			if (pressed)
				*v = !(*v);

			static std::map<ImGuiID, float> grab_anim;
			auto it_grab = grab_anim.find(id);

			if (it_grab == grab_anim.end())
			{
				grab_anim.insert({ id, 20.f });
				it_grab = grab_anim.find(id);
			}

			if (*v)
			{
				if (it_grab->second < 20.f)
					it_grab->second += 1.5f;
			}
			else
			{
				if (it_grab->second > 5.f)
					it_grab->second -= 1.5f;
			}

			ImVec4 grabcol_active(33 / 255.f, 150 / 255.f, 243 / 255.f, 255 / 255.f);
			ImVec4 grabcol_inactive(98 / 255.f, 101 / 255.f, 106 / 255.f, 255 / 255.f);

			static std::map<ImGuiID, ImVec4> grabcol_anim;
			auto it_grabcol = grabcol_anim.find(id);

			if (it_grabcol == grabcol_anim.end())
			{
				grabcol_anim.insert({ id, grabcol_active });
				it_grabcol = grabcol_anim.find(id);
			}

			if (*v)
			{
				if (it_grabcol->second.x < grabcol_active.x)
					it_grabcol->second.x += 0.03f;
				if (it_grabcol->second.x > grabcol_active.x)
					it_grabcol->second.x -= 0.03f;

				if (it_grabcol->second.y < grabcol_active.y)
					it_grabcol->second.y += 0.03f;
				if (it_grabcol->second.y > grabcol_active.y)
					it_grabcol->second.y -= 0.03f;

				if (it_grabcol->second.z < grabcol_active.z)
					it_grabcol->second.z += 0.03f;
				if (it_grabcol->second.z > grabcol_active.z)
					it_grabcol->second.z -= 0.03f;
			}
			else
			{
				if (it_grabcol->second.x < grabcol_inactive.x)
					it_grabcol->second.x += 0.03f;
				if (it_grabcol->second.x > grabcol_inactive.x)
					it_grabcol->second.x -= 0.03f;

				if (it_grabcol->second.y < grabcol_inactive.y)
					it_grabcol->second.y += 0.03f;
				if (it_grabcol->second.y > grabcol_inactive.y)
					it_grabcol->second.y -= 0.03f;

				if (it_grabcol->second.z < grabcol_inactive.z)
					it_grabcol->second.z += 0.03f;
				if (it_grabcol->second.z > grabcol_inactive.z)
					it_grabcol->second.z -= 0.03f;
			}

			window->DrawList->AddRectFilled(total_bb.Min + ImVec2(0, 1), ImVec2(total_bb.Min.x + 25, total_bb.Max.y - 1), ImColor(152, 166, 168), 360);
			window->DrawList->AddRectFilled(total_bb.Min + ImVec2(0, 1), ImVec2(total_bb.Min.x + it_grab->second, total_bb.Max.y - 1), ImColor(146, 198, 243), 360);
			window->DrawList->AddCircleFilled(ImVec2(total_bb.Min.x + it_grab->second, total_bb.Min.y + 6), 7, ImColor(it_grabcol->second.x, it_grabcol->second.y, it_grabcol->second.z, 1.f), 360);

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229 / 255.f, 229 / 255.f, 229 / 255.f, 255 / 255.f));
			ImGui::RenderText(ImVec2(total_bb.Min.x + 35, total_bb.Min.y + 12 / 2 - label_size.y / 2), label);
			ImGui::PopStyleColor();
			return pressed;
		}

		//BUTTON
		bool button_2(const char* label, const ImVec2& size_arg)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

			ImVec2 pos = window->DC.CursorPos;
			ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

			const ImRect bb(pos, pos + size);
			ImGui::ItemSize(size, style.FramePadding.y);
			if (!ImGui::ItemAdd(bb, id))
				return false;

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

			if (hovered || held)
				ImGui::SetMouseCursor(0);


			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229 / 255.f, 229 / 255.f, 229 / 255.f, 255 / 255.f));
			ImGui::RenderText(ImVec2(bb.Min.x + style.ItemInnerSpacing.x + 0, bb.Min.y + style.FramePadding.y + 0), label);
			ImGui::PopStyleColor();

			return pressed;
		}

		//SLIDER
		static const char* PatchFormatStringFloatToInt(const char* fmt)
		{
			if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0)
				return "%d";

			const char* fmt_start = ImParseFormatFindStart(fmt);
			const char* fmt_end = ImParseFormatFindEnd(fmt_start);
			if (fmt_end > fmt_start && fmt_end[-1] == 'f')
			{
				if (fmt_start == fmt && fmt_end[0] == 0)
					return "%d";
				ImGuiContext& g = *GImGui;
				ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end);
				return g.TempBuffer;
			}
			return fmt;
		}

		bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const float w = ImGui::CalcItemWidth() + ImGui::CalcItemWidth() * 0.25f;

			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
			const ImRect frame_bb(window->DC.CursorPos + ImVec2(0, 20), window->DC.CursorPos + ImVec2(w, 30));
			const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

			ImGui::ItemSize(total_bb, style.FramePadding.y);
			if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
				return false;

			if (format == NULL)
				format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;
			else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0)
				format = PatchFormatStringFloatToInt(format);
			const bool hovered = ImGui::ItemHoverable(frame_bb, id);
			const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
			bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
			if (!temp_input_is_active)
			{
				const bool focus_requested = temp_input_allowed && ImGui::FocusableItemRegister(window, id);
				const bool clicked = (hovered && g.IO.MouseClicked[0]);
				if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
				{
					ImGui::SetActiveID(id, window);
					ImGui::SetFocusID(id, window);
					ImGui::FocusWindow(window);
					g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
				}
			}

			ImRect grab_bb;
			const bool value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);

			window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x, frame_bb.Min.y + 3), ImVec2(frame_bb.Max.x, frame_bb.Max.y - 3), ImColor(152, 166, 168), 360);
			window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x, frame_bb.Min.y + 3), ImVec2(grab_bb.Min.x, frame_bb.Max.y - 3), ImColor(146, 198, 243), 360);
			window->DrawList->AddCircleFilled(ImVec2(grab_bb.Min.x, frame_bb.Min.y + 5), 5, ImColor(33 / 255.f, 150 / 255.f, 243 / 255.f, 255 / 255.f), 360);

			char value_buf[64];
			const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

			ImGui::RenderText(ImVec2(total_bb.Max.x - 5 - ImGui::CalcTextSize(value_buf).x, total_bb.Min.y), value_buf);
			ImGui::RenderText(ImVec2(total_bb.Min.x + 5, total_bb.Min.y), label);
			return value_changed;
		}

		bool slider_float(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
		{
			return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
		}

		bool slider_int(const char* label, int* v, int v_min, int v_max, const char* format = NULL, ImGuiSliderFlags flags = NULL)
		{
			return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
		}

		//COMBOBOX
		static float CalcMaxPopupHeightFromItemCount(int items_count)
		{
			ImGuiContext& g = *GImGui;
			if (items_count <= 0)
				return FLT_MAX;
			return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
		}

		bool combo_box(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
		bool combo_box(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
		bool combo_box(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

		bool begincombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
		{

			ImGuiContext& g = *GImGui;
			bool has_window_size_constraint = (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint) != 0;
			g.NextWindowData.Flags &= ~ImGuiNextWindowDataFlags_HasSizeConstraint;

			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);

			const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : ImGui::GetFrameHeight();
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
			const float expected_w = ImGui::CalcItemWidth();
			const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : expected_w;

			const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + 0, label_size.y + style.FramePadding.y * 2.0f + 0));
			const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

			ImGui::ItemSize(total_bb, style.FramePadding.y);
			if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
				return false;

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(frame_bb, id, &hovered, &held);
			bool popup_open = ImGui::IsPopupOpen(id, ImGuiPopupFlags_None);

			if (hovered || held)
				ImGui::SetMouseCursor(0);

			const float value_x2 = ImMax(frame_bb.Min.x, frame_bb.Max.x - arrow_size);

			window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x + 0, frame_bb.Min.y + 0), ImVec2(frame_bb.Max.x + 0, frame_bb.Max.y + 0), ImColor(0.176471f, 0.196078f, 0.219608f, 1.000000f), 0, 15);
			if (!(flags & ImGuiComboFlags_NoArrowButton))
			{
				ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
				if (value_x2 + arrow_size - style.FramePadding.x <= frame_bb.Max.x)
					ImGui::RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y + 0, frame_bb.Min.y + style.FramePadding.y + 0), text_col, ImGuiDir_Down, 10);
			}

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229 / 255.f, 229 / 255.f, 229 / 255.f, 255 / 255.f));
			if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
				ImGui::RenderText(frame_bb.Min + style.FramePadding + ImVec2(0, 0), preview_value);
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229 / 255.f, 229 / 255.f, 229 / 255.f, 255 / 255.f));
			if (label_size.x > 0)
				ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x + 0, frame_bb.Min.y + style.FramePadding.y + 0), label);
			ImGui::PopStyleColor();

			if ((pressed || g.NavActivateId == id) && !popup_open)
			{
				if (window->DC.NavLayerCurrent == 0)
					window->NavLastIds[0] = id;
				ImGui::OpenPopupEx(id, ImGuiPopupFlags_None);
				popup_open = true;
			}

			if (!popup_open)
				return false;

			if ((flags & ImGuiComboFlags_HeightMask_) == 0)
				flags |= ImGuiComboFlags_HeightRegular;
			IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_));
			int popup_max_height_in_items = -1;
			if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
			else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
			else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
			ImGui::SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

			char name[16];
			ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size);

			if (ImGuiWindow* popup_window = ImGui::FindWindowByName(name))
				if (popup_window->WasActive)
				{
					ImVec2 size_expected = ImGui::CalcWindowExpectedSize(popup_window);
					if (flags & ImGuiComboFlags_PopupAlignLeft)
						popup_window->AutoPosLastDirection = ImGuiDir_Left;
					ImRect r_outer = ImGui::GetWindowAllowedExtentRect(popup_window);
					ImVec2 pos = ImGui::FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
					ImGui::SetNextWindowPos(pos);
				}

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
			bool ret = ImGui::Begin(name, NULL, window_flags);
			ImGui::PopStyleVar();

			if (!ret)
			{
				ImGui::EndPopup();
				return false;
			}
			return true;
		}

		static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
		{
			const char* const* items = (const char* const*)data;
			if (out_text)
				*out_text = items[idx];
			return true;
		}

		static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
		{
			// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
			const char* items_separated_by_zeros = (const char*)data;
			int items_count = 0;
			const char* p = items_separated_by_zeros;
			while (*p)
			{
				if (idx == items_count)
					break;
				p += strlen(p) + 1;
				items_count++;
			}
			if (!*p)
				return false;
			if (out_text)
				*out_text = p;
			return true;
		}

		bool combo_box(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
		{

			ImGuiContext& g = *GImGui;
			const char* preview_value = NULL;
			if (*current_item >= 0 && *current_item < items_count)
				items_getter(data, *current_item, &preview_value);
			if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
				SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
			if (!begincombo(label, preview_value, ImGuiComboFlags_None))
				return false;
			bool value_changed = false;

			for (int i = 0; i < items_count; i++)
			{
				PushID((void*)(intptr_t)i);
				const bool item_selected = (i == *current_item);
				const char* item_text;
				if (!items_getter(data, i, &item_text))
					item_text = " * Unknown item * ";
				if (Selectable(item_text, item_selected))
				{
					value_changed = true;
					*current_item = i;
				}
				if (item_selected)
					SetItemDefaultFocus();
				PopID();
			}

			EndCombo();
			return value_changed;
		}

		bool combo_box(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
		{
			const bool value_changed = combo_box(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
			return value_changed;
		}

		bool combo_box(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
		{
			int items_count = 0;
			const char* p = items_separated_by_zeros;

			while (*p)
			{
				p += strlen(p) + 1;
				items_count++;
			}

			bool value_changed = combo_box(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
			return value_changed;
		}

		//SUBTAB
		bool sub_tab(const char* label, bool selected)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

			ImVec2 pos = window->DC.CursorPos;
			ImVec2 size = ImGui::CalcItemSize({ 100, 35 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

			const ImRect bb(pos, pos + size);
			ImGui::ItemSize(size, style.FramePadding.y);
			if (!ImGui::ItemAdd(bb, id))
				return false;

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

			if (hovered || held)
				ImGui::SetMouseCursor(0);

			ImVec4 text_active(33 / 255.f, 150 / 255.f, 243 / 255.f, 255 / 255.f);
			ImVec4 text_inactive(98 / 255.f, 101 / 255.f, 106 / 255.f, 255 / 255.f);

			static std::map<ImGuiID, ImVec4> text_anim;
			auto it_text = text_anim.find(id);

			if (it_text == text_anim.end())
			{
				text_anim.insert({ id, text_active });
				it_text = text_anim.find(id);
			}

			if (selected)
			{
				if (it_text->second.x < text_active.x)
					it_text->second.x += 0.02f;
				if (it_text->second.x > text_active.x)
					it_text->second.x -= 0.02f;

				if (it_text->second.y < text_active.y)
					it_text->second.y += 0.02f;
				if (it_text->second.y > text_active.y)
					it_text->second.y -= 0.02f;

				if (it_text->second.z < text_active.z)
					it_text->second.z += 0.02f;
				if (it_text->second.z > text_active.z)
					it_text->second.z -= 0.02f;
			}
			else
			{
				if (it_text->second.x < text_inactive.x)
					it_text->second.x += 0.02f;
				if (it_text->second.x > text_inactive.x)
					it_text->second.x -= 0.02f;

				if (it_text->second.y < text_inactive.y)
					it_text->second.y += 0.02f;
				if (it_text->second.y > text_inactive.y)
					it_text->second.y -= 0.02f;

				if (it_text->second.z < text_inactive.z)
					it_text->second.z += 0.02f;
				if (it_text->second.z > text_inactive.z)
					it_text->second.z -= 0.02f;
			}

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(it_text->second.x, it_text->second.y, it_text->second.z, 255 / 255.f));
			ImGui::RenderText(ImVec2(bb.Min.x + 44 / 2, bb.Min.y + 35 / 2), label);
			ImGui::PopStyleColor();

			return pressed;
		}

		bool tab(const char* label, bool selected, bool icon = true)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

			ImVec2 pos = window->DC.CursorPos;
			ImVec2 size = ImGui::CalcItemSize({ 44, 35 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

			const ImRect bb(pos, pos + size);
			ImGui::ItemSize(size, style.FramePadding.y);

			if (!ImGui::ItemAdd(bb, id))
				return false;

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

			if (hovered || held)
				ImGui::SetMouseCursor(0);

			ImVec4 text_active(33 / 255.f, 150 / 255.f, 243 / 255.f, 255 / 255.f);
			ImVec4 text_inactive(98 / 255.f, 101 / 255.f, 106 / 255.f, 255 / 255.f);

			static std::map<ImGuiID, ImVec4> text_anim;
			auto it_text = text_anim.find(id);

			if (it_text == text_anim.end())
			{
				text_anim.insert({ id, text_active });
				it_text = text_anim.find(id);
			}

			if (selected)
			{
				if (it_text->second.x < text_active.x)
					it_text->second.x += 0.02f;
				if (it_text->second.x > text_active.x)
					it_text->second.x -= 0.02f;

				if (it_text->second.y < text_active.y)
					it_text->second.y += 0.02f;
				if (it_text->second.y > text_active.y)
					it_text->second.y -= 0.02f;

				if (it_text->second.z < text_active.z)
					it_text->second.z += 0.02f;
				if (it_text->second.z > text_active.z)
					it_text->second.z -= 0.02f;
			}
			else
			{
				if (it_text->second.x < text_inactive.x)
					it_text->second.x += 0.02f;
				if (it_text->second.x > text_inactive.x)
					it_text->second.x -= 0.02f;

				if (it_text->second.y < text_inactive.y)
					it_text->second.y += 0.02f;
				if (it_text->second.y > text_inactive.y)
					it_text->second.y -= 0.02f;

				if (it_text->second.z < text_inactive.z)
					it_text->second.z += 0.02f;
				if (it_text->second.z > text_inactive.z)
					it_text->second.z -= 0.02f;
			}

			ImGui::PushFont(icons);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(it_text->second.x, it_text->second.y, it_text->second.z, 255 / 255.f));
			ImGui::RenderText(ImVec2(bb.Min.x + (44 / 2 - label_size.x / 2), bb.Min.y + (35 / 2 - label_size.y / 2)), label);
			ImGui::PopStyleColor();
			ImGui::PopFont();

			return pressed;
		}

		bool buffering_bar(const char* label, float value, const ImVec2& pos_arg, const ImVec2& size_arg, const ImVec4& bg_col, const ImVec4& fg_col) {

			ImGuiWindow* window = GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);

			ImVec2 pos = pos_arg;
			ImVec2 size = size_arg;

			//size.x -= style.FramePadding.x * 2;

			const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
			ItemSize(bb, style.FramePadding.y);

			if (!ItemAdd(bb, id))
				return false;

			// Render

			window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + size.x, bb.Max.y), ImGui::GetColorU32(bg_col));
			window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + size.x * value, bb.Max.y), ImGui::GetColorU32(fg_col));
		}

		bool button(const char* label, ImVec2 size_args = ImVec2(0, 0)) {

			ImGuiWindow* window = GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = CalcTextSize(label, NULL, true);

			ImVec2 pos = window->DC.CursorPos;
			ImVec2 size = CalcItemSize(size_args, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

			const ImRect bb(pos, pos + size);
			ItemSize(size, style.FramePadding.y);
			if (!ItemAdd(bb, id))
				return false;

			bool hovered, held;
			bool pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

			// Render
			const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
			RenderNavHighlight(bb, id);
			RenderFrame(bb.Min, bb.Max, col, false, 5.f);
			RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

			IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
			return pressed;
		}

		bool image_button(ImTextureID texId, const char* label, const ImVec2& imageSize, int frame_padding, int width = -1, int warning_col = 0) {
			ImGuiWindow* window = GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImVec2 size = imageSize;
			if (size.x <= 0 && size.y <= 0) { size.x = size.y = ImGui::GetTextLineHeightWithSpacing(); }
			else {
				if (size.x <= 0)          size.x = size.y;
				else if (size.y <= 0)     size.y = size.x;
				size *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
			}

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;

			const ImGuiID id = window->GetID(label);
			const ImVec2 textSize = ImGui::CalcTextSize(label, NULL, true);
			const bool hasText = textSize.x > 0;

			const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
			const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
			const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y); \

				const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + (width < 0 ? totalSizeWithoutPadding : ImVec2(width, totalSizeWithoutPadding.y)) + padding * 2);

			ImVec2 start(0, 0);
			start = window->DC.CursorPos + padding; if (size.y < textSize.y) start.y += (textSize.y - size.y) * .5f;
			const ImRect image_bb(start, start + size);
			start = window->DC.CursorPos + padding; start.x += size.x + innerSpacing; if (size.y > textSize.y) start.y += (size.y - textSize.y) * .5f;

			ItemSize(bb);

			if (!ItemAdd(bb, id))
				return false;

			bool hovered = false, held = false;
			bool pressed = ButtonBehavior(bb, id, &hovered, &held);

			// Render
			const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
			RenderFrame(bb.Min, bb.Max, col, false, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, 5.f));
			window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max);

			if (textSize.x > 0) {

				switch (warning_col) {
				case 0: break;
				case 1:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5, 0.2, 1));
					break;
				case 2:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.2, 0.2, 1));
					break;
				}

				ImGui::RenderText(start, label);

				if (warning_col > 0)
					ImGui::PopStyleColor();
			}
			return pressed;
		}

		const char* const key_name_list[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
		};

		bool keybind(const char* label, int* k, const ImVec2& size_arg)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			ImGuiIO& io = g.IO;
			const ImGuiStyle& style = g.Style;

			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
			ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
			const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), window->DC.CursorPos + size);
			const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

			ImGui::ItemSize(total_bb, style.FramePadding.y);
			if (!ImGui::ItemAdd(total_bb, id))
				return false;

			const bool focus_requested = ImGui::FocusableItemRegister(window, id);


			const bool hovered = ImGui::ItemHoverable(frame_bb, id);

			if (hovered) {
				ImGui::SetHoveredID(id);
				g.MouseCursor = ImGuiMouseCursor_TextInput;
			}

			const bool user_clicked = hovered && io.MouseClicked[0];

			if (focus_requested || user_clicked) {
				if (g.ActiveId != id) {
					// Start edition
					memset(io.MouseDown, 0, sizeof(io.MouseDown));
					memset(io.KeysDown, 0, sizeof(io.KeysDown));
					*k = 0;
				}
				ImGui::SetActiveID(id, window);
				ImGui::FocusWindow(window);
			}
			else if (io.MouseClicked[0]) {
				// Release focus when we click outside
				if (g.ActiveId == id)
					ImGui::ClearActiveID();
			}

			bool value_changed = false;
			int key = *k;

			if (g.ActiveId == id) {
				for (auto i = 0; i < 5; i++) {
					if (io.MouseDown[i]) {
						switch (i) {
						case 0:
							key = VK_LBUTTON;
							break;
						case 1:
							key = VK_RBUTTON;
							break;
						case 2:
							key = VK_MBUTTON;
							break;
						case 3:
							key = VK_XBUTTON1;
							break;
						case 4:
							key = VK_XBUTTON2;
							break;
						}
						value_changed = true;
						ImGui::ClearActiveID();
					}
				}
				if (!value_changed) {
					for (auto i = VK_BACK; i <= VK_RMENU; i++) {
						if (io.KeysDown[i]) {
							key = i;
							value_changed = true;
							ImGui::ClearActiveID();
						}
					}
				}

				if (IsKeyPressedMap(ImGuiKey_Escape)) {
					*k = 0;
					ImGui::ClearActiveID();
				}
				else {
					*k = key;
				}
			}

			// Render
			// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

			char buf_display[64] = "None";

			ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImVec4(0.20f, 0.25f, 0.30f, 1.0f)), true, style.FrameRounding);

			if (*k != 0 && g.ActiveId != id) {
				strcpy_s(buf_display, key_name_list[*k]);
			}
			else if (g.ActiveId == id) {
				strcpy_s(buf_display, "<Press a key>");
			}

			const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
			ImVec2 render_pos = frame_bb.Min + style.FramePadding;
			ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);

			if (label_size.x > 0)
				ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

			return value_changed;
		}
	}
}