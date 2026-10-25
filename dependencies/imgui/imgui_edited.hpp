#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_internal.h"
#include "imgui_settings.h"
#include "imstb_textedit.h"
#include "../cstrike/core/menu.h"

#include <cstdlib>
#include "imgui.h"

#include <map>

#include <string>
#include <wtypes.h>
#include "../cstrike/sdk/datatypes/color.h"

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")
namespace edited
{ 
    bool            BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);
    bool            BeginChild(const char* str_id, const ImVec2& size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);
    void            EndChild();
    bool            BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags);
    bool            TabEx(ID3D11ShaderResourceView* ic0, const char* label, const char* hint, bool selected, ImFont* iconfont, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0, Color_t color = {0,0,0,0});
    bool            Tab(ID3D11ShaderResourceView* icon, const char* label, const char* description, bool selected, ImFont* iconfont, const ImVec2& size_arg, Color_t color);
    bool            Checkbox(const char* label, const char* description, bool* v);
    bool            Button(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags);
    bool            group(const char* label, bool selected, Color_t color);
    bool            QuandScalar(const char* label, const char* description, ImGuiDataType data_type, void* p_data0, void* p_data1, const void* p_min0, const void* p_max0, const void* p_min1, const void* p_max1, const char* format0 = "%d", const char* format1 = "%d", ImGuiSliderFlags flags = 0);
    bool            QuandFloat(const char* label, const char* description, float* v0, float* v1, float v_min0 = 0.0f, float v_max0 = 0.0f, float v_min1 = 0.0f, float v_max1 = 0.0f, const char* format0 = "%d", const char* format1 = "%d", ImGuiSliderFlags flags = 0);
    bool            QuandInt(const char* label, const char* description, int* v0, int* v1, int v_min0 = 0, int v_max0 = 0, int v_min1 = 0, int v_max1 = 0, const char* format0 = "%d", const char* format1 = "%d", ImGuiSliderFlags flags = 0);

    bool            SliderInt(const char* label, const char* description, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool            SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
    bool            SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    bool            pointbox(const char* label, bool* v, int type, float x, float y);
 
    bool           Color(const char* szLabel, const char* szDesc, Color_t* pColor, ImGuiColorEditFlags flags);
    bool           ColorEdit4(const char* label, const char* description, float col[4], ImGuiColorEditFlags flags = 0);
    bool           ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    bool           ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0, 0));

     bool           SmallCheckbox(const char* label, bool* v, int type);
     bool		    Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
     bool		    Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
     bool		    BeginCombo2(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
     bool		    BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
     void		    EndCombo();
     void           MultiCombo(const char* label, bool variable[], const char* labels[], int count);
     bool           Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
     bool           Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
     bool           Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);
     bool           RangeSliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v1, float* v2, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags = 0);
     bool           RangeSliderFloat(const char* label, const char* description, float* v1, float* v2, float v_min, float v_max, const char* display_format = "(%.3f, %.3f)", float power = 1.0f);

     void           esp_preview(ImTextureID player_preview, bool* nickname, float nick_color[4], bool* weapon, float weapon_color[4], int* hp, float hp_color[4], bool* zoom, float zoom_color[4], bool* bomb, float bomb_color[4], bool* c4, float c4_color[4], bool* money, float money_color[4], bool* hit, float hit_color[4], bool* esp_box, float box_color[4], bool* hp_line, float hp_line_color[4]);

     bool           checkboxP(const char* label, bool* v, const ImVec2& size_arg);
     bool           Keybind(const char* label, const char* description, int* key);

     ///////////////////////////////////////////// TABS

     inline ImVec4 tab_text_active = ImColor(255, 255, 255, 255);
     inline ImVec4 tab_text_hov = ImColor(98, 112, 122, 255);
     inline ImVec4 tab_text = ImColor(88, 92, 112, 255);

     inline ImVec4 tab_active = ImColor(88, 109, 202, 255);
     inline ImVec4 glow_active = ImColor(21, 226, 96, 230);

     ///////////////////////////////////////////// TEXT

     inline ImVec4 text_active = ImColor(255, 255, 255, 255);
     inline ImVec4 text_hov = ImColor(76, 75, 80, 255);
     inline ImVec4 text = ImColor(66, 65, 70, 255);

     ///////////////////////////////////////////// CHILD

     inline ImVec4 child_background = ImColor(22, 21, 26, 255);
     inline ImVec4 border_child = ImColor(28, 27, 32, 255);
     inline ImVec4 child_gradient = ImColor(196, 55, 64);

     inline ImVec4 border_child_default = ImColor(22, 21, 26, 255);
     inline ImVec4 child_name = ImColor(62, 61, 65, 255);

     ///////////////////////////////////////////// GRADIENT

     inline ImVec4 checkbox_gradient_start = ImColor(196, 55, 64, 150);
     inline ImVec4 checkbox_gradient_end = ImColor(216, 55, 64, 150);

     ///////////////////////////////////////////// CHECKBOXES

     inline ImVec4 checkbox_bg = ImColor(17, 16, 21, 150);
     inline ImVec4 checkbox_mark = ImColor(255, 255, 255, 150);

     ///////////////////////////////////////////// SLIDERS

     inline ImVec4 slider_background = ImColor(17, 16, 21, 255);
     inline ImVec4 circle_push_gradient = ImColor(255, 255, 255, 255);
     inline ImVec4 circle_pop_gradient = ImColor(255, 255, 255, 255);

     ///////////////////////////////////////////// INPUTTEXT

     inline ImVec4 input_push_gradient = ImColor(27, 26, 31, 255);
     inline ImVec4 input_pop_gradient = ImColor(17, 16, 21, 255);


     ///////////////////////////////////////////// SCROLLBAR

     inline ImVec4 scroll_background = ImColor(15, 17, 20, 255);
     inline ImVec4 scroll_keep = ImColor(36, 35, 40, 255);

     ///////////////////////////////////////////// COMBOBOX

     inline ImVec4 combo_background = ImColor(28, 27, 32, 255);

     inline ImVec4 combo_push_gradient = ImColor(27, 26, 31, 255);
     inline ImVec4 combo_pop_gradient = ImColor(17, 16, 21, 255);

     ///////////////////////////////////////////// BUTTON

     inline ImVec4 button_push_gradient_active = ImColor(27, 26, 31, 255);
     inline ImVec4 button_pop_gradient_active = ImColor(17, 16, 21, 255);

     inline ImVec4 button_push_gradient_hovered = ImColor(30, 29, 34, 255);
     inline ImVec4 button_pop_gradient_hovered = ImColor(19, 20, 24, 255);

     inline ImVec4 button_push_gradient = ImColor(27, 26, 31, 255);
     inline ImVec4 button_pop_gradient = ImColor(17, 16, 21, 255);

     ///////////////////////////////////////////// KEYBIND

     inline ImVec4 keybind_background = ImColor(18, 18, 20, 150);
     inline ImVec4 keybind_border = ImColor(16, 16, 18, 150);

     ///////////////////////////////////////////// TAB VISIBLE

     inline ImVec4 tab_push_gradient = ImColor(0, 145, 92, 255);
     inline ImVec4 tab_pop_gradient = ImColor(21, 226, 96, 255);
}