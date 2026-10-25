#include "imgui_edited.hpp"
#include <vector>

using namespace ImGui;

namespace edited
{

    const char* keys[] =
    {
        "-",
        "M1",
        "M2",
        "CN",
        "M3",
        "M4",
        "M5",
        "-",
        "BACK",
        "TAB",
        "-",
        "-",
        "CLR",
        "ENTER",
        "-",
        "-",
        "SHIFT",
        "CTRL",
        "Menu",
        "Pause",
        "CAPS",
        "KAN",
        "-",
        "JUN",
        "FIN",
        "KAN",
        "-",
        "ESC",
        "CON",
        "NCO",
        "ACC",
        "MAD",
        "SPACE",
        "PGU",
        "PGD",
        "END",
        "HOME",
        "LEFT",
        "UP",
        "RIGHT",
        "DOWN",
        "SEL",
        "PRI",
        "EXE",
        "PRI",
        "INS",
        "DEL",
        "HEL",
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
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
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
        "WIN",
        "WIN",
        "APP",
        "-",
        "SLE",
        "NUM0",
        "NUM1",
        "NUM2",
        "NUM3",
        "NUM4",
        "NUM5",
        "NUM6",
        "NUM7",
        "NUM8",
        "NUM9",
        "MUL",
        "ADD",
        "SEP",
        "MIN",
        "DEL",
        "DIV",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "NUM",
        "SCR",
        "EQU",
        "MAS",
        "TOY",
        "OYA",
        "OYA",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "SHIFT",
        "SHIFT",
        "CTRL",
        "CTRL",
        "ALT",
        "ALT"
    };

#include <string>

    void RenderTextColor(ImFont* font, const ImVec2& p_min, const ImVec2& p_max, ImU32 col, const char* text, const ImVec2& align)
    {
        PushFont(font);
        PushStyleColor(ImGuiCol_Text, col);
        RenderTextClipped(p_min, p_max, text, NULL, NULL, align, NULL);
        PopStyleColor();
        PopFont();
    }

    struct keybind_state {
        ImVec4 background_color;
        ImVec4 rect_color;
        ImVec2 key_size;
        bool mode;
        const char* key_name;
        ImVec4 mode_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 key_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 text_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        float alpha = 0.f;
    };
    bool Keybind(const char* label, const char* description, int* key)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        ImGuiIO& io = g.IO;
        const ImGuiStyle& style = g.Style;

        ImVec2 mode_size;
        ImVec2 key_size;

        SetCursorPos(GetCursorPos() - ImVec2(0, 0));

        const ImGuiID id = window->GetID(label);
        const float size = GetWindowWidth() - 36;
        const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size, 30));
        ImRect mode_rect(window->DC.CursorPos + ImVec2(size - 0, 0), window->DC.CursorPos + ImVec2(size, 40));
        ImRect key_rect;

        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + ImVec2(270, 35));

        ImGui::ItemSize(rect, style.FramePadding.y);
        if (!ImGui::ItemAdd(rect, id))
            return false;

        bool popup_open = ImGui::IsPopupOpen(id, ImGuiPopupFlags_None);
        bool hovered_rect = ItemHoverable(rect, id, NULL);

        bool value_changed = false;
        int k = *key;

        mode_rect.Min.x -= mode_size.x;
        key_rect = ImRect(mode_rect.Min - ImVec2(CalcItemWidth(), 0), ImVec2(mode_rect.Min.x - 1, mode_rect.Max.y));

        bool hovered_mode = ItemHoverable(mode_rect, id, NULL);

        char buf_display1[64] = "None";

        std::string active_key = "";
        active_key += keys[*key];

        if (*key != 0 && g.ActiveId != id) {
            strcpy_s(buf_display1, active_key.c_str());
        }
        else if (g.ActiveId == id) {
            strcpy_s(buf_display1, "-");
        }
        key_size = CalcTextSize(buf_display1, NULL, true);
        key_rect.Min.x -= key_size.x;

        bool hovered_key = ItemHoverable(key_rect, id, NULL);

        std::string nametwo = "keybind_state";
        nametwo += label;


        if (hovered_key && io.MouseClicked[0])
        {
            if (g.ActiveId != id) {
                // Start edition
                memset(io.MouseDown, 0, sizeof(io.MouseDown));
                memset(io.KeysDown, 0, sizeof(io.KeysDown));
                *key = 0;
            }
            ImGui::SetActiveID(id, window);
            ImGui::FocusWindow(window);
        }
        else if (io.MouseClicked[0]) {
            // Release focus when we click outside
            if (g.ActiveId == id)
                ImGui::ClearActiveID();
        }

        if (g.ActiveId == id) {
            for (auto i = 0; i < 5; i++) {
                if (io.MouseDown[i]) {
                    switch (i) {
                    case 0:
                        k = 0x01;
                        break;
                    case 1:
                        k = 0x02;
                        break;
                    case 2:
                        k = 0x04;
                        break;
                    case 3:
                        k = 0x05;
                        break;
                    case 4:
                        k = 0x06;
                        break;
                    }
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
            if (!value_changed) {
                for (auto i = 0x08; i <= 0xA5; i++) {
                    if (io.KeysDown[i]) {
                        k = i;
                        value_changed = true;
                        ImGui::ClearActiveID();
                    }
                }
            }

            if (IsKeyPressedMap(ImGuiKey_Escape)) {
                *key = 0;
                ImGui::ClearActiveID();
            }
            else {
                *key = k;
            }
        }

        static std::map<ImGuiID, keybind_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, keybind_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.text_color = ImLerp(it_anim->second.text_color, IsItemActive() ? tab_text_active : hovered_rect ? tab_text_hov : text, g.IO.DeltaTime * 6.f);

        static std::map<ImGuiID, float> alpha_animation;
        auto it_alpha = alpha_animation.find(id);
        if (it_alpha == alpha_animation.end())
        {
            alpha_animation.insert({ id, 0.f });
            it_alpha = alpha_animation.find(id);
        }

        it_alpha->second = ImLerp(it_alpha->second, hovered_rect ? 0.2f : 0.f, g.IO.DeltaTime * 6.f);

        it_alpha->second *= style.Alpha;

        window->DrawList->AddText(rect.Min + ImVec2(0, 4), GetColorU32(it_anim->second.text_color), label);

        PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text_color));

        window->DrawList->AddRectFilled(key_rect.Min + ImVec2(CalcItemWidth() - 21, -1), key_rect.Max - ImVec2(2, 12), GetColorU32(keybind_background), style.FrameRounding);
        window->DrawList->AddRect(key_rect.Min + ImVec2(CalcItemWidth() - 21, -1), key_rect.Max - ImVec2(2, 12), GetColorU32(keybind_border), style.FrameRounding);

        PopStyleColor();

        RenderTextClipped(key_rect.Min - ImVec2(60, 31), key_rect.Max + ImVec2(CalcItemWidth() + 38, 16), buf_display1, NULL, NULL, ImVec2(0.5f, 0.5f));

        SetCursorPos(GetCursorPos() - ImVec2(0, 10));

        return value_changed;
    }

    struct checkp_state
    {
        ImVec4 background;
    };

    bool checkboxP(const char* label, bool* v, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true), pos = window->DC.CursorPos;

        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

        const ImRect bb(pos, pos + size);

        ItemSize(size, 0.f);
        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);
        if (IsItemClicked())
        {
            *v = !(*v);
            MarkItemEdited(id);
        }

        static std::map<ImGuiID, checkp_state> anim;
        checkp_state& state = anim[id];

        state.background = ImLerp(state.background, *v ? c::accent : c::elements::background, g.IO.DeltaTime * 6.f);
        auto walpha = GetColorU32(ImVec4(c::elements::background.x, c::elements::background.y, c::elements::background.z, 1.f));
        auto nalpha = GetColorU32(ImVec4(c::elements::background.x, c::elements::background.y, c::elements::background.z, 0.1f));
        ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max,nalpha, 3.f);
        ImGui::GetWindowDrawList()->AddRect(bb.Min, bb.Max, GetColorU32(state.background), 2.f);

        RenderTextColor(font::lexend_regular, bb.Min, bb.Max, GetColorU32(c::elements::text_active), label, ImVec2(0.5, 0.5));

        return pressed;
    }

    //struct tab_state
    //{
    //    ImVec4 background, text, icon, shadow;
    //    float bg_alpha;
    //};


    struct selection_anim
    {
        int hovered_anim;
        int active_anim;
    };

    bool edited::TabEx(ID3D11ShaderResourceView* ic0, const char* label, const char* hint, bool selected, ImFont* iconfont, const ImVec2& size_arg, ImGuiButtonFlags flags, Color_t color)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        ImVec2 pos = window->DC.CursorPos;

        const ImRect rect(pos, pos + ImVec2(50, 50));
        ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
        if (!ImGui::ItemAdd(rect, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

        static std::map <ImGuiID, selection_anim> anim;
        auto it_anim = anim.find(id);
        if (it_anim == anim.end())
        {
            anim.insert({ id, {0, 0} });
            it_anim = anim.find(id);
        }

        if (hovered)
            it_anim->second.hovered_anim += 8 * (1.f - ImGui::GetIO().DeltaTime);
        else
            it_anim->second.hovered_anim -= 8 * (1.f - ImGui::GetIO().DeltaTime);

        if (it_anim->second.hovered_anim > 50)
            it_anim->second.hovered_anim = 50;
        else if (it_anim->second.hovered_anim < 0)
            it_anim->second.hovered_anim = 0;

        if (selected)
            it_anim->second.active_anim += 16 * (1.f - ImGui::GetIO().DeltaTime);
        else
            it_anim->second.active_anim -= 16 * (1.f - ImGui::GetIO().DeltaTime);

        if (it_anim->second.active_anim > 155)
            it_anim->second.active_anim = 155;
        else if (it_anim->second.active_anim < 0)
            it_anim->second.active_anim = 0;

        //int hovered_anim = animate(label, "hovered_anim", hovered, 50, 8, STATIC);

        //int active_anim = animate(label, "active_anim", selected, 155, 16, STATIC);

        window->DrawList->AddImage(ic0, rect.Min + ImVec2(15, 15), rect.Max - ImVec2(15, 15), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 100 + it_anim->second.hovered_anim + it_anim->second.active_anim));

        return pressed;
    }

    bool edited::Tab(ID3D11ShaderResourceView* icon, const char* label, const char* description, bool selected, ImFont* iconfont, const ImVec2& size_arg, Color_t color)
    {
        return TabEx(icon, label, description, selected, iconfont, size_arg, ImGuiButtonFlags_None, color);
    }

    bool edited::BeginChild(const char* str_id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags)
    {
        ImGuiID id = GetCurrentWindow()->GetID(str_id);

        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * dpi, 10 * dpi));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10 * dpi, 10 * dpi));

        return edited::BeginChildEx(str_id, id, size_arg * dpi, child_flags, window_flags | ImGuiWindowFlags_AlwaysUseWindowPadding);
    }

    bool edited::BeginChild(ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags)
    {
        return edited::BeginChildEx(NULL, id, size_arg, child_flags, window_flags);
    }

    bool edited::BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;
        IM_ASSERT(id != 0);

        const ImGuiChildFlags ImGuiChildFlags_SupportedMask_ = ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle;
        IM_UNUSED(ImGuiChildFlags_SupportedMask_);
        IM_ASSERT((child_flags & ~ImGuiChildFlags_SupportedMask_) == 0 && "Illegal ImGuiChildFlags value. Did you pass ImGuiWindowFlags values instead of ImGuiChildFlags?");
        IM_ASSERT((window_flags & ImGuiWindowFlags_AlwaysAutoResize) == 0 && "Cannot specify ImGuiWindowFlags_AlwaysAutoResize for BeginChild(). Use ImGuiChildFlags_AlwaysAutoResize!");
        if (child_flags & ImGuiChildFlags_AlwaysAutoResize)
        {
            IM_ASSERT((child_flags & (ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY)) == 0 && "Cannot use ImGuiChildFlags_ResizeX or ImGuiChildFlags_ResizeY with ImGuiChildFlags_AlwaysAutoResize!");
            IM_ASSERT((child_flags & (ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY)) != 0 && "Must use ImGuiChildFlags_AutoResizeX or ImGuiChildFlags_AutoResizeY with ImGuiChildFlags_AlwaysAutoResize!");
        }
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        if (window_flags & ImGuiWindowFlags_AlwaysUseWindowPadding) child_flags |= ImGuiChildFlags_AlwaysUseWindowPadding;
#endif
        if (child_flags & ImGuiChildFlags_AutoResizeX) child_flags &= ~ImGuiChildFlags_ResizeX;
        if (child_flags & ImGuiChildFlags_AutoResizeY) child_flags &= ~ImGuiChildFlags_ResizeY;

        window_flags |= ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_NoTitleBar;
        window_flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);
        if (child_flags & (ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize)) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
        if ((child_flags & (ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY)) == 0) window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

        if (child_flags & ImGuiChildFlags_FrameStyle)
        {
            PushStyleColor(ImGuiCol_ChildBg, g.Style.Colors[ImGuiCol_FrameBg]);
            PushStyleVar(ImGuiStyleVar_ChildRounding, g.Style.FrameRounding);
            PushStyleVar(ImGuiStyleVar_ChildBorderSize, g.Style.FrameBorderSize);
            PushStyleVar(ImGuiStyleVar_WindowPadding, g.Style.FramePadding);
            child_flags |= ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding;
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        g.NextWindowData.Flags |= ImGuiNextWindowDataFlags_HasChildFlags;
        g.NextWindowData.ChildFlags = child_flags;

        const ImVec2 size_avail = GetContentRegionAvail();
        const ImVec2 size_default((child_flags & ImGuiChildFlags_AutoResizeX) ? 0.0f : size_avail.x, (child_flags & ImGuiChildFlags_AutoResizeY) ? 0.0f : size_avail.y);
        const ImVec2 size = CalcItemSize(size_arg, size_default.x, size_default.y);
        SetNextWindowSize(size * dpi);

   //     GetWindowDrawList()->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + size_arg, GetColorU32(c::accent), 0);

        const char* temp_window_name;

        if (name)
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
        else
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

        const float backup_border_size = g.Style.ChildBorderSize;
        if ((child_flags & ImGuiChildFlags_Border) == 0) g.Style.ChildBorderSize = 0.0f;

        const bool ret = Begin(temp_window_name, NULL, window_flags);

        g.Style.ChildBorderSize = backup_border_size;
        if (child_flags & ImGuiChildFlags_FrameStyle)
        {
            PopStyleVar(3);
            PopStyleColor();
        }

        ImGuiWindow* child_window = g.CurrentWindow;
        child_window->ChildId = id;

        if (child_window->BeginCount == 1) parent_window->DC.CursorPos = child_window->Pos;

        const ImGuiID temp_id_for_activation = ImHashStr("##Child", 0, id);
        if (g.ActiveId == temp_id_for_activation) ClearActiveID();
        if (g.NavActivateId == id && !(window_flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY))
        {
            FocusWindow(child_window);
            NavInitWindow(child_window, false);
            SetActiveID(temp_id_for_activation, child_window);
            g.ActiveIdSource = g.NavInputSource;
        }
        return ret;
    }

    void edited::EndChild()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* child_window = g.CurrentWindow;
        PopStyleVar(2);
        IM_ASSERT(g.WithinEndChild == false);
        IM_ASSERT(child_window->Flags & ImGuiWindowFlags_ChildWindow);

        g.WithinEndChild = true;
        ImVec2 child_size = child_window->Size;
        End();
        if (child_window->BeginCount == 1)
        {
            ImGuiWindow* parent_window = g.CurrentWindow;
            ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + child_size);
            ItemSize(child_size);
            if ((child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY) && !(child_window->Flags & ImGuiWindowFlags_NavFlattened))
            {
                ItemAdd(bb, child_window->ChildId);
                RenderNavHighlight(bb, child_window->ChildId);

                if (child_window->DC.NavLayersActiveMask == 0 && child_window == g.NavWindow) RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
            }
            else
            {
                ItemAdd(bb, 0);

                if (child_window->Flags & ImGuiWindowFlags_NavFlattened) parent_window->DC.NavLayersActiveMaskNext |= child_window->DC.NavLayersActiveMaskNext;
            }
            if (g.HoveredWindow == child_window) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
        }
        g.WithinEndChild = false;
        g.LogLinePosY = -FLT_MAX;
    }

    struct checkbox_state {

        ImVec4 gradient_color0, gradient_color1;

        ImVec4 text_color, checkmark_color, background_color;

        float rg;
    };

    bool edited::Checkbox(const char* label, const char* description, bool* v)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const float square_sz = GetFrameHeight() + 1;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id))
        {
            IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
            return false;
        }

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (IsItemClicked())
        {
            *v = !(*v);
            MarkItemEdited(id);
        }

        static std::map<ImGuiID, checkbox_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, checkbox_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.gradient_color0 = ImLerp(it_anim->second.gradient_color0, *v ? ImColor{ MENU::gay_menucol.GetU32() } : hovered ? ImColor(0, 0, 0, 0) : ImColor(0, 0, 0, 0), g.IO.DeltaTime * 6.f);
        it_anim->second.gradient_color1 = ImLerp(it_anim->second.gradient_color1, *v ? ImColor{ MENU::gay_menucol.GetU32() } : hovered ? ImColor(0, 0, 0, 0) : ImColor(0, 0, 0, 0), g.IO.DeltaTime * 6.f);
        it_anim->second.checkmark_color = ImLerp(it_anim->second.checkmark_color, *v ? ImColor{ checkbox_mark } : hovered ? ImColor(0, 0, 0, 0) : ImColor(0, 0, 0, 0), g.IO.DeltaTime * 9.f);
        it_anim->second.rg = ImLerp(it_anim->second.rg, *v ? 0.f : 10.f, g.IO.DeltaTime * 5.f);


        it_anim->second.text_color = ImLerp(it_anim->second.text_color, *v ? text_active : hovered ? text_hov : text, g.IO.DeltaTime * 6.f);

        const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
        RenderNavHighlight(total_bb, id);

        RenderFrame(check_bb.Min, check_bb.Max, GetColorU32(checkbox_bg), true, style.FrameRounding);

        const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
        window->DrawList->AddRectFilled(check_bb.Min + ImVec2(it_anim->second.rg, it_anim->second.rg), check_bb.Max - ImVec2(it_anim->second.rg, it_anim->second.rg), GetColorU32(it_anim->second.gradient_color0), style.FrameRounding);
        ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetWindowDrawList(), vtx_idx_1, ImGui::GetWindowDrawList()->VtxBuffer.Size, check_bb.Min, check_bb.Max, GetColorU32(it_anim->second.gradient_color1), GetColorU32(it_anim->second.gradient_color0));

        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
        RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad + 3 *  dpi, pad + 3 * dpi + it_anim->second.rg), GetColorU32(it_anim->second.checkmark_color), square_sz - pad * 4.0f);

        ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);

        window->DrawList->AddText(label_pos, GetColorU32(it_anim->second.text_color), label);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return pressed;
    }

    struct button_state {

        ImVec4 background_color;

    };

    bool edited::Button(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
            pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
        ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ItemSize(size, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
            flags |= ImGuiButtonFlags_Repeat;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

        static std::map<ImGuiID, button_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, button_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.background_color = ImLerp(it_anim->second.background_color, IsItemActive() ? c::child::Button_Active : hovered ? c::child::Button_Hovered : c::child::Button, g.IO.DeltaTime * 6.f);

        RenderFrame(bb.Min, bb.Max, GetColorU32(it_anim->second.background_color), true, style.FrameRounding);


        RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed;
    }

    struct group_anim
    {
        int hovered_text_anim;
        int active_text_anim;
        int active_outline_anim;
        int active_rect_alpha;
    };

    bool group(const char* label, bool selected, Color_t color)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true) - ImVec2(0.f, 2.f);
        ImVec2 pos = window->DC.CursorPos;

        const ImRect rect(pos, pos + ImVec2(label_size.x + 60.f, 25.f));
        ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 2.f, rect.Max.y), style.FramePadding.y);
        if (!ImGui::ItemAdd(rect, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

        static std::map <ImGuiID, group_anim> anim;
        auto it_anim = anim.find(id);
        if (it_anim == anim.end())
        {
            anim.insert({ id, {0, 0, 0, 0} });
            it_anim = anim.find(id);
        }

        if (hovered)
            it_anim->second.hovered_text_anim += 8 * (1.f - ImGui::GetIO().DeltaTime);
        else
            it_anim->second.hovered_text_anim -= 8 * (1.f - ImGui::GetIO().DeltaTime);

        if (it_anim->second.hovered_text_anim > 50)
            it_anim->second.hovered_text_anim = 50;
        else if (it_anim->second.hovered_text_anim < 0)
            it_anim->second.hovered_text_anim = 0;

        if (selected)
            it_anim->second.active_text_anim += 16 * (1.f - ImGui::GetIO().DeltaTime);
        else
            it_anim->second.active_text_anim -= 16 * (1.f - ImGui::GetIO().DeltaTime);

        if (it_anim->second.active_text_anim > 155)
            it_anim->second.active_text_anim = 155;
        else if (it_anim->second.active_text_anim < 0)
            it_anim->second.active_text_anim = 0;

        if (selected)
            it_anim->second.active_outline_anim += 3 * (1.f - ImGui::GetIO().DeltaTime);
        else
            it_anim->second.active_outline_anim -= 3 * (1.f - ImGui::GetIO().DeltaTime);

        if (it_anim->second.active_outline_anim > 15)
            it_anim->second.active_outline_anim = 15;
        else if (it_anim->second.active_outline_anim < 0)
            it_anim->second.active_outline_anim = 0;

        if (selected)
            it_anim->second.active_rect_alpha += 20 * (1.f - ImGui::GetIO().DeltaTime);
        else
            it_anim->second.active_rect_alpha -= 20 * (1.f - ImGui::GetIO().DeltaTime);

        if (it_anim->second.active_rect_alpha > 255)
            it_anim->second.active_rect_alpha = 255;
        else if (it_anim->second.active_rect_alpha < 0)
            it_anim->second.active_rect_alpha = 0;

        //int hovered_text_anim = animate(label, "hovered_text_anim", hovered, 50, 8, STATIC);

        //int active_text_anim = animate(label, "active_text_anim", selected, 155, 16, STATIC);

        //int active_outline_anim = animate(label, "active_outline_anim", selected, 15, 3, STATIC);

        //int active_rect_alpha = animate(label, "active_rect_alpha", selected, 255, 20, STATIC);

        window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(11, 11, 11, 0.7), 5.f);

        window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(color.r, color.g, color.b, 170), 5.f);

       window->DrawList->AddRect(rect.Min, rect.Max, ImColor(255, 255, 255, (15 + it_anim->second.active_outline_anim)), 5.f);

        window->DrawList->AddText(ImVec2((rect.Min.x + rect.Max.x) / 2.f - (label_size.x / 2.f), (rect.Min.y + rect.Max.y) / 2.f - (label_size.y / 2.f)), ImColor(255, 255, 255, (100 + it_anim->second.hovered_text_anim + it_anim->second.active_text_anim)), label);


        return pressed;
    }

    struct slider_state {

        ImVec4 background_color, background_slider, circle_color, text_color;
        float slow, hint_slider;
    };

    bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        SetCursorPos(GetCursorPos() + ImVec2(0, 16));

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = CalcItemWidth();

        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + 54, label_size.y + 5));
        const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

        const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
            return false;

        if (format == NULL)
            format = DataTypeGetInfo(data_type)->PrintFmt;

        const bool hovered = ItemHoverable(frame_bb, id, NULL);
        bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
        if (!temp_input_is_active)
        {

            const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
            const bool clicked = hovered && IsMouseClicked(0, id);
            const bool make_active = (input_requested_by_tabbing || clicked || g.NavActivateId == id || g.NavActivateInputId == id);
            if (make_active && clicked)
                SetKeyOwner(ImGuiKey_MouseLeft, id);
            if (make_active && temp_input_allowed)
                if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || g.NavActivateInputId == id)
                    temp_input_is_active = true;

            if (make_active && !temp_input_is_active)
            {
                SetActiveID(id, window);
                SetFocusID(id, window);
                FocusWindow(window);
                g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
            }
        }

        if (temp_input_is_active);

        ImRect grab_bb;

        static std::map<ImGuiID, slider_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, slider_state() });
            it_anim = anim.find(id);
        }

        //  it_anim->second.background_color = ImLerp(it_anim->second.background_color, IsItemActive() ? slider_active : hovered ? slider_hov : slider, g.IO.DeltaTime * 6.f);
        //  it_anim->second.background_slider = ImLerp(it_anim->second.background_slider, IsItemActive() ? bg_slider_active : hovered ? bg_slider_hov : bg_slider, g.IO.DeltaTime * 6.f);

        it_anim->second.text_color = ImLerp(it_anim->second.text_color, IsItemActive() ? text_active : hovered ? text_hov : text, g.IO.DeltaTime * 12.f);
        it_anim->second.hint_slider = ImLerp(it_anim->second.hint_slider, IsItemHovered() ? 1.f : 0.f, g.IO.DeltaTime * 35.f);

        window->DrawList->AddRectFilled(frame_bb.Min + ImVec2(0, 8 * dpi), frame_bb.Max - ImVec2(0, 3), GetColorU32(slider_background), 100);

        const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
        if (value_changed) MarkItemEdited(id);

        if (grab_bb.Max.x > grab_bb.Min.x) {

            it_anim->second.slow = ImLerp(it_anim->second.slow, grab_bb.Min.x + 5 - frame_bb.Min.x, g.IO.DeltaTime * 20.f);

            const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
            window->DrawList->AddRectFilled(frame_bb.Min + ImVec2(0, 8), frame_bb.Min + ImVec2(it_anim->second.slow - 7, 16.5f), GetColorU32(checkbox_gradient_start), 100, ImDrawFlags_RoundCornersLeft);
            ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetWindowDrawList(), vtx_idx_1, ImGui::GetWindowDrawList()->VtxBuffer.Size, frame_bb.Min, frame_bb.Max, GetColorU32(checkbox_gradient_start), GetColorU32(checkbox_gradient_end));

            const int vtx_idx_2 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
            window->DrawList->AddCircleFilled(frame_bb.Min + ImVec2(it_anim->second.slow - 1, 12.f), 8, GetColorU32(circle_push_gradient), 100);
            ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetWindowDrawList(), vtx_idx_2, ImGui::GetWindowDrawList()->VtxBuffer.Size, frame_bb.Min, frame_bb.Min + ImVec2(7, 10), GetColorU32(circle_push_gradient), GetColorU32(circle_pop_gradient));
        }

        char value_buf[64];
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text_color));
        RenderTextClipped(frame_bb.Min - ImVec2(0, 20), frame_bb.Max - ImVec2(0, 20), value_buf, value_buf_end, NULL, ImVec2(1.0f, 0.5f));
        if (label_size.x > 0.0f) RenderText(ImVec2(frame_bb.Max.x - w - 54, frame_bb.Min.y - 20), label);
        PopStyleColor();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return value_changed;
    }

    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
    }

    bool SliderInt(const char* label, const char* description, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
    }


    struct begin_state
    {
        ImVec4 background, text, outline;
        float open, alpha, combo_size = 0.f, shadow_opticaly;
        bool opened_combo = false, hovered = false;
        float arrow_roll;
    };

    static float CalcMaxPopupHeightFromItemCount(int items_count)
    {
        ImGuiContext& g = *GImGui;
        if (items_count <= 0)
            return FLT_MAX;
        return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
    }

    int rotation_start_index;
    void ImRotateStart()
    {
        rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    }

    ImVec2 ImRotationCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

        const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
    }

    void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
    {
        float s = sin(rad), c = cos(rad);
        center = ImRotate(center, s, c) - center;

        auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
    }

    static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
    {
        const char* const* items = (const char* const*)data;
        if (out_text)
            *out_text = items[idx];
        return true;
    }
    
    bool BeginCombo2(const char* label, const char* preview_value, ImGuiComboFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();

        g.NextWindowData.ClearFlags();
        if (window->SkipItems) return false;

        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const float w = ((GetContentRegionMax().x - style.WindowPadding.x));
        const float y_size = 50;

        const ImRect bb(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(w, 50));
        const ImRect rect(window->DC.CursorPos + ImVec2(180, (50 - 30) / 2), window->DC.CursorPos + ImVec2(w - 10, (50 + 30) / 2));

        const ImRect total_bb(bb.Min, bb.Max);
        ItemSize(bb, 0.f);

        if (!ItemAdd(bb, id, &bb)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held);

        static std::map<ImGuiID, begin_state> anim;
        begin_state& state = anim[id];

        if (hovered && g.IO.MouseClicked[0] || state.opened_combo && g.IO.MouseClicked[0] && !state.hovered) state.opened_combo = !state.opened_combo;

        state.arrow_roll = ImLerp(state.arrow_roll, state.opened_combo ? -1.f : 1.f, g.IO.DeltaTime * 6.f);
        state.text = ImLerp(state.text, state.opened_combo ? c::elements::text_active : hovered ? c::elements::text_hov : c::elements::text, g.IO.DeltaTime * 6.f);
        //   state.combo_size = ImLerp(state.combo_size, state.opened_combo ? (val * 33) + 17 : 0.f, g.IO.DeltaTime * 12.f);

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(c::elements::background), c::elements::rounding);

        //RenderTextColor(font::lexend_regular, bb.Min + ImVec2(10, 0), bb.Max, GetColorU32(c::elements::text), description, ImVec2(0.0, 0.8));

        GetWindowDrawList()->AddRectFilled(bb.Min + ImVec2(w - 140, 0), bb.Max, GetColorU32(c::elements::background), c::elements::rounding);
        GetWindowDrawList()->AddRectFilledMultiColor(bb.Min, bb.Max - ImVec2(120, 0), VGetColorU32(c::elements::background, 0.f), VGetColorU32(c::elements::background, 1.f), VGetColorU32(c::elements::background, 1.f), VGetColorU32(c::elements::background, 0.f));

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::elements::background_widget), c::elements::rounding);

        RenderTextColor(font::lexend_general_bold, rect.Min + ImVec2(10, 0), rect.Max, GetColorU32(c::elements::text_active), preview_value, ImVec2(0.0, 0.5));
        GetWindowDrawList()->AddRectFilled(rect.Max - ImVec2(30, 30), rect.Max, GetColorU32(c::elements::background_widget), 2.f);

        GetWindowDrawList()->AddRectFilledMultiColor(rect.Min, rect.Max - ImVec2(30, 0), VGetColorU32(c::elements::background_widget, 0.f), VGetColorU32(c::elements::background_widget, 1.f), VGetColorU32(c::elements::background_widget, 1.f), VGetColorU32(c::elements::background_widget, 0.f));

        RenderTextColor(font::icomoon_widget, rect.Min, rect.Max - ImVec2(7, 0), GetColorU32(c::accent), "z", ImVec2(1.0, 0.5));
        RenderTextColor(font::lexend_general_bold, bb.Min + ImVec2(10, 0), bb.Max, GetColorU32(c::elements::text_active), label, ImVec2(0.0, 0.2));

        GetWindowDrawList()->AddRectFilled(rect.Max - ImVec2(y_size, y_size), rect.Max, GetColorU32(state.background), c::elements::rounding);

        if (!IsRectVisible(rect.Min, rect.Max + ImVec2(0, 2)))
        {
            state.opened_combo = false;
            state.combo_size = 0.f;
        }

        if (!state.opened_combo && state.combo_size < 2.f) return false;

        ImGui::SetNextWindowPos(ImVec2(rect.Min.x, rect.Max.y + 5));
        ImGui::SetNextWindowSize(ImVec2(rect.GetWidth(), state.combo_size));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollWithMouse;

        PushStyleColor(ImGuiCol_WindowBg, c::elements::background);
        PushStyleColor(ImGuiCol_Border, c::elements::background_widget);
        PushStyleVar(ImGuiStyleVar_WindowRounding, c::elements::rounding);
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
        PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);

        bool ret = Begin(label, NULL, window_flags);

        PopStyleVar(3);
        PopStyleColor(2);

        state.hovered = IsWindowHovered();

        return true;
    }

    struct combo_state
    {
        ImVec4 background_color;
        ImVec4 arrow_color;
        ImVec4 text;
        float expand_rotation = 1.0f;
    };

    bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
    {

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();

        ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
        g.NextWindowData.ClearFlags();
        if (window->SkipItems)
            return false;

        SetCursorPos(GetCursorPos() + ImVec2(0, 25));

        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

        const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : CalcItemWidth();
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + 40, label_size.y + style.FramePadding.y * 5.5f));
        const ImRect total_bb(bb.Min, bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &bb))
            return false;

        static std::map<ImGuiID, combo_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, combo_state() });
            it_anim = anim.find(id);
        }

        // Open on click
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);
        const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
        bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
        if (pressed && !popup_open)
        {
            OpenPopupEx(popup_id, ImGuiPopupFlags_None);
            popup_open = true;
        }

        it_anim->second.background_color = ImLerp(it_anim->second.background_color, popup_open ? c::child::Combo_Active : hovered ? c::child::Combo_Hovered : c::child::Combo, g.IO.DeltaTime * 6.f);
        it_anim->second.text = ImLerp(it_anim->second.text, popup_open ? c::child::Text_Active : hovered ? c::child::Text_Hovered : c::child::Text, g.IO.DeltaTime * 6.f);
        it_anim->second.expand_rotation = ImLerp(it_anim->second.expand_rotation, popup_open ? -1.f : 1.f, g.IO.DeltaTime * 10.f);

        const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);
        RenderNavHighlight(bb, id);
        if (!(flags & ImGuiComboFlags_NoPreview))

         window->DrawList->AddRectFilled(bb.Min, ImVec2(value_x2 + 34 * dpi, bb.Max.y), GetColorU32(it_anim->second.background_color), style.FrameRounding);

        //window->DrawList->AddRectFilled(bb.Min + ImVec2(200, 0), ImVec2(value_x2 - 3, bb.Max.y + 1), GetColorU32(ImGuiCol_ChildBg), 0);

        if (flags & ImGuiComboFlags_CustomPreview)
        {
            g.ComboPreviewData.PreviewRect = ImRect(bb.Min.x, bb.Min.y, value_x2, bb.Max.y);
            IM_ASSERT(preview_value == NULL || preview_value[0] == 0);
            preview_value = NULL;
        }

        PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text));

        if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
        {
            RenderTextClipped(bb.Min + ImVec2(5, 5) + style.FramePadding, ImVec2(value_x2 + 20 * dpi, bb.Max.y), preview_value, NULL, NULL);
            ImRotateStart();
            PushFont(font::icomoon);
            RenderTextClipped(bb.Min + ImVec2(w + 14, 0) + style.FramePadding, ImVec2(value_x2 + 25 * dpi, bb.Max.y), "E", NULL, NULL);
            PopFont();
            ImRotateEnd(1.57f * it_anim->second.expand_rotation);
        }

        if (label_size.x > 0) RenderText(ImVec2(bb.Max.x - 40 - w, bb.Min.y - 32 + style.FramePadding.y), label);
        PopStyleColor();

        if (!popup_open)
            return false;

        g.NextWindowData.Flags = backup_next_window_data_flags;
        return BeginComboPopup(popup_id, bb, flags);
    }

    void EndCombo()
    {
        End();
    }

    struct comboA_state
    {
        bool active = false;
        float alpha = 0.f;
    };

    struct multi_state
    {
        bool active = false;
        float alpha = 0.f;
    };

    void MultiCombo(const char* label, bool variable[], const char* labels[], int count)
    {
        ImGuiContext& g = *GImGui;

        std::string preview = "None";

        std::vector<std::string> vec;


        for (auto i = 0, j = 0; i < count; i++)
        {
            if (variable[i])
            {
                vec.push_back(labels[i]);

                if (j >= 3)
                    preview = vec.at(0) + ", " + vec.at(1) + ", " + vec.at(2) + "...";
                else if (j)
                    preview += ", " + (std::string)labels[i];
                else
                    preview = labels[i];

                j++;
            }
        }
        if (edited::BeginCombo(label, preview.c_str(), 0)) // draw start
        {

            static std::map<ImGuiID, comboA_state> anim;
            auto it_anim = anim.find(g.CurrentWindow->ID);

            if (it_anim == anim.end())
            {
                anim.insert({ g.CurrentWindow->ID, comboA_state() });
                it_anim = anim.find(g.CurrentWindow->ID);
            }

            if (ImGuiWindow* popup_window = ImGui::FindWindowByName(g.CurrentWindow->Name))
                if (popup_window->WasActive)
                {
                    it_anim->second.active = true;
                }
                else
                {
                    it_anim->second.active = false;
                    it_anim->second.alpha = 0.f;
                }

            it_anim->second.alpha = ImClamp(it_anim->second.alpha + (8.f * ImGui::GetIO().DeltaTime * (it_anim->second.active * it_anim->second.active ? 1.f : -1.f)), 0.f, 1.f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 7));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 7));
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, it_anim->second.alpha);
            for (auto i = 0; i < count; i++)
                ImGui::Selectable(labels[i], &variable[i], ImGuiSelectableFlags_DontClosePopups);
            ImGui::PopStyleVar(3);
            ImGui::EndCombo();
        }

        preview = ("None"); // reset preview to use later
    }

    bool BeginComboPreview()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

        if (window->SkipItems || !(g.LastItemData.StatusFlags & ImGuiItemStatusFlags_Visible)) return false;

        IM_ASSERT(g.LastItemData.Rect.Min.x == preview_data->PreviewRect.Min.x && g.LastItemData.Rect.Min.y == preview_data->PreviewRect.Min.y);

        if (!window->ClipRect.Overlaps(preview_data->PreviewRect)) return false;

        preview_data->BackupCursorPos = window->DC.CursorPos;
        preview_data->BackupCursorMaxPos = window->DC.CursorMaxPos;
        preview_data->BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
        preview_data->BackupPrevLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
        preview_data->BackupLayout = window->DC.LayoutType;
        window->DC.CursorPos = preview_data->PreviewRect.Min + g.Style.FramePadding;
        window->DC.CursorMaxPos = window->DC.CursorPos;
        window->DC.LayoutType = ImGuiLayoutType_Horizontal;
        window->DC.IsSameLine = false;
        PushClipRect(preview_data->PreviewRect.Min, preview_data->PreviewRect.Max, true);

        return true;
    }

    void EndComboPreview()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

        ImDrawList* draw_list = window->DrawList;
        if (window->DC.CursorMaxPos.x < preview_data->PreviewRect.Max.x && window->DC.CursorMaxPos.y < preview_data->PreviewRect.Max.y)
            if (draw_list->CmdBuffer.Size > 1)
            {
                draw_list->_CmdHeader.ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 2].ClipRect;
                draw_list->_TryMergeDrawCmds();
            }
        PopClipRect();
        window->DC.CursorPos = preview_data->BackupCursorPos;
        window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, preview_data->BackupCursorMaxPos);
        window->DC.CursorPosPrevLine = preview_data->BackupCursorPosPrevLine;
        window->DC.PrevLineTextBaseOffset = preview_data->BackupPrevLineTextBaseOffset;
        window->DC.LayoutType = preview_data->BackupLayout;
        window->DC.IsSameLine = false;
        preview_data->PreviewRect = ImRect();
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

    bool Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
    {
        ImGuiContext& g = *GImGui;

        // Call the getter to obtain the preview string which is a parameter to BeginCombo()
        const char* preview_value = NULL;
        if (*current_item >= 0 && *current_item < items_count)
            items_getter(data, *current_item, &preview_value);

        // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
        if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
            SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

        if (!BeginCombo(label, preview_value, ImGuiComboFlags_None))
            return false;

        // Display items
        // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
        bool value_changed = false;
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 7));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 7));
        for (int i = 0; i < items_count; i++)
        {
            PushID(i);
            const bool item_selected = (i == *current_item);
            const char* item_text;
            if (!items_getter(data, i, &item_text))
                item_text = "*Unknown item*";
            if (Selectable(item_text, item_selected))
            {
                value_changed = true;
                *current_item = i;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }
        PopStyleVar(2);
        EndCombo();
        if (value_changed)
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

    bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
    {
        const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
        return value_changed;
    }

    bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
    {
        int items_count = 0;
        const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
        while (*p)
        {
            p += strlen(p) + 1;
            items_count++;
        }
        bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
        return value_changed;
    }

    struct select_state
    {
        ImVec4 text;
        float opticaly;
    };

    bool Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        ImGuiID id = window->GetID(label);
        ImVec2 label_size = CalcTextSize(label, NULL, true);
        ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
        ImVec2 pos = window->DC.CursorPos;
        pos.y += window->DC.CurrLineTextBaseOffset;
        ItemSize(size, 0.0f);

        const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
        const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
        const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
        if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth)) size.x = ImMax(label_size.x, max_x - min_x);

        const ImVec2 text_min = pos;
        const ImVec2 text_max(min_x + size.x, pos.y + size.y);

        ImRect bb(min_x, pos.y, text_max.x, text_max.y);
        if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
        {
            const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
            const float spacing_y = style.ItemSpacing.y;
            const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
            const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
            bb.Min.x -= spacing_L;
            bb.Min.y -= spacing_U;
            bb.Max.x += (spacing_x - spacing_L);
            bb.Max.y += (spacing_y - spacing_U);
        }

        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        if (span_all_columns)
        {
            window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
            window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        }

        const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
        const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
        if (span_all_columns)
        {
            window->ClipRect.Min.x = backup_clip_rect_min_x;
            window->ClipRect.Max.x = backup_clip_rect_max_x;
        }

        if (!item_add) return false;

        const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
        if (disabled_item && !disabled_global) BeginDisabled();

        if (span_all_columns && window->DC.CurrentColumns) PushColumnsBackground();
        else if (span_all_columns && g.CurrentTable) TablePushBackgroundChannel();

        ImGuiButtonFlags button_flags = 0;
        if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
        if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
        if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
        if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
        if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
        if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) { button_flags |= ImGuiButtonFlags_AllowOverlap; }

        const bool was_selected = selected;
        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

        if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
            if (g.NavJustMovedToId == id)  selected = pressed = true;

        // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
        if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
        {
            if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
            {
                SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
                g.NavDisableHighlight = true;
            }
        }
        if (pressed) MarkItemEdited(id);

        if (selected != was_selected)  g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;


        if (g.NavId == id) RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

        if (span_all_columns && window->DC.CurrentColumns) PopColumnsBackground();
        else if (span_all_columns && g.CurrentTable) TablePopBackgroundChannel();

        static std::map<ImGuiID, select_state> anim;
        select_state& state = anim[id];

        state.text = ImLerp(state.text, selected ? c::accent : c::elements::text, g.IO.DeltaTime * 6.f);
        state.opticaly = ImLerp(state.opticaly, selected ? 1.f : 0.f, g.IO.DeltaTime * 6.f);

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max + ImVec2(0, 1), VGetColorU32(c::elements::background_widget, state.opticaly), 2.f);

        RenderTextColor(font::lexend_bold, text_min + ImVec2(1, 1), text_max, GetColorU32(state.text), label, ImVec2(0.f, 0.5f));

        if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup)) CloseCurrentPopup();

        if (disabled_item && !disabled_global) EndDisabled();

        return pressed;
    }

    bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
    {
        if (Selectable(label, *p_selected, flags, size_arg))
        {
            *p_selected = !*p_selected;
            return true;
        }
        return false;
    }

    int ParseFormatPrecision(const char* fmt, int default_precision)
    {
        int precision = default_precision;
        while ((fmt = strchr(fmt, '%')) != NULL)
        {
            fmt++;
            if (fmt[0] == '%') { fmt++; continue; } // Ignore "%%"
            while (*fmt >= '0' && *fmt <= '9')
                fmt++;
            if (*fmt == '.')
            {
                precision = atoi(fmt + 1);
                if (precision < 0 || precision > 10)
                    precision = default_precision;
            }
            break;
        }
        return precision;
    }

    float RoundScalar(float value, int decimal_precision)
    {
        static const float min_steps[10] = { 1.0f, 0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f, 0.000001f, 0.0000001f, 0.00000001f, 0.000000001f };
        float min_step = (decimal_precision >= 0 && decimal_precision < 10) ? min_steps[decimal_precision] : powf(10.0f, (float)-decimal_precision);
        bool negative = value < 0.0f;
        value = fabsf(value);
        float remainder = fmodf(value, min_step);
        if (remainder <= min_step * 0.5f)
            value -= remainder;
        else
            value += (min_step - remainder);
        return negative ? -value : value;
    }

    static inline float SliderBehaviorCalcRatioFromValue(float v, float v_min, float v_max, float power, float linear_zero_pos)
    {
        if (v_min == v_max)
            return 0.0f;

        const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
        const float v_clamped = (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
        if (is_non_linear)
        {
            if (v_clamped < 0.0f)
            {
                const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f, v_max) - v_min);
                return (1.0f - powf(f, 1.0f / power)) * linear_zero_pos;
            }
            else
            {
                const float f = (v_clamped - ImMax(0.0f, v_min)) / (v_max - ImMax(0.0f, v_min));
                return linear_zero_pos + powf(f, 1.0f / power) * (1.0f - linear_zero_pos);
            }
        }

        return (v_clamped - v_min) / (v_max - v_min);
    }

    struct range_state
    {
        ImVec4 text;
        float slow1, slow2, alpha_text, text_offset;
    };

    bool RangeSliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v1, float* v2, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        const ImGuiStyle& style = g.Style;

        const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
        const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;

        const float grab_padding = 2.0f;
        const float slider_sz = is_horizontal ? (frame_bb.GetWidth() - grab_padding * 2.0f) : (frame_bb.GetHeight() - grab_padding * 2.0f);
        float grab_sz;
        if (decimal_precision > 0)
            grab_sz = ImMin(style.GrabMinSize, slider_sz);
        else
            grab_sz = ImMin(ImMax(1.0f * (slider_sz / ((v_min < v_max ? v_max - v_min : v_min - v_max) + 1.0f)), style.GrabMinSize), slider_sz);
        const float slider_usable_sz = slider_sz - grab_sz;
        const float slider_usable_pos_min = (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding + grab_sz * 0.5f;
        const float slider_usable_pos_max = (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding - grab_sz * 0.5f;

        float linear_zero_pos = 0.0f;
        if (v_min * v_max < 0.0f)
        {
            const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
            const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
            linear_zero_pos = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
        }
        else
        {
            linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
        }

        window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(c::elements::background_widget), 30.f);

        bool value_changed = false;
        if (g.ActiveId == id)
        {
            if (g.IO.MouseDown[0])
            {
                const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
                float clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
                if (!is_horizontal)
                    clicked_t = 1.0f - clicked_t;

                float new_value;
                if (is_non_linear)
                {
                    if (clicked_t < linear_zero_pos)
                    {
                        float a = 1.0f - (clicked_t / linear_zero_pos);
                        a = powf(a, power);
                        new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
                    }
                    else
                    {
                        float a;
                        if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
                            a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
                        else
                            a = clicked_t;
                        a = powf(a, power);
                        new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
                    }
                }
                else
                {
                    new_value = ImLerp(v_min, v_max, clicked_t);
                }
                new_value = RoundScalar(new_value, decimal_precision);
                if (*v1 != new_value || *v2 != new_value)
                {
                    if (fabsf(*v1 - new_value) < fabsf(*v2 - new_value))
                    {
                        *v1 = new_value;
                    }
                    else
                    {
                        *v2 = new_value;
                    }
                    value_changed = true;
                }
            }
            else
            {
                SetActiveID(0, NULL);
            }
        }

        float grab_t = SliderBehaviorCalcRatioFromValue(*v1, v_min, v_max, power, linear_zero_pos);

        static std::map<ImGuiID, range_state> anim;
        range_state& state = anim[id];

        ImRect grab_bb2, grab_bb1;

        if (!is_horizontal) grab_t = 1.0f - grab_t;
        float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
        if (is_horizontal)
            grab_bb1 = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
        else
            grab_bb1 = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));

        state.slow1 = ImLerp(state.slow1, grab_bb1.Min.x - frame_bb.Min.x - 1, g.IO.DeltaTime * 25.f);
        grab_t = SliderBehaviorCalcRatioFromValue(*v2, v_min, v_max, power, linear_zero_pos);
        if (!is_horizontal) grab_t = 1.0f - grab_t;

        grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);

        if (is_horizontal)
            grab_bb2 = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
        else
            grab_bb2 = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));

        state.slow2 = ImLerp(state.slow2, grab_bb2.Min.x - frame_bb.Min.x + 14, g.IO.DeltaTime * 25.f);

        window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x + state.slow1, frame_bb.Min.y), ImVec2(frame_bb.Min.x + state.slow2, frame_bb.Max.y), GetColorU32(c::accent), 2.f);

        return value_changed;
    }

    bool RangeSliderFloat(const char* label, const char* description, float* v1, float* v2, float v_min, float v_max, const char* display_format, float power)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = GetContentRegionMax().x - style.WindowPadding.x;
        const float y_size = 50;

        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, y_size));
        const ImRect slider_bb(window->DC.CursorPos + ImVec2(180, 31), window->DC.CursorPos + ImVec2(w - 10, 39));

        if (!ItemAdd(slider_bb, id))
        {
            ItemSize(ImRect(rect.Min, rect.Max));
            return false;
        }

        const bool hovered = ItemHoverable(slider_bb, id, g.LastItemData.InFlags);

        if (hovered) SetHoveredID(id);

        if (!display_format) display_format = "(%.3f, %.3f)";

        ItemSize(ImRect(rect.Min, rect.Max));

        int decimal_precision = ParseFormatPrecision(display_format, 3);

        if (hovered && g.IO.MouseClicked[0])
        {
            SetActiveID(id, window);
            FocusWindow(window);
        }

        static std::map<ImGuiID, range_state> anim;
        range_state& state = anim[id];

        state.text = ImLerp(state.text, IsItemActive() ? c::elements::text_active : hovered ? c::elements::text_hov : c::elements::text, g.IO.DeltaTime * 8.f);
        state.alpha_text = ImLerp(state.alpha_text, IsItemActive() ? 1.f : 0.f, g.IO.DeltaTime * 15.f);

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::elements::background), c::elements::rounding);

        RenderTextColor(font::lexend_regular, rect.Min + ImVec2(10, 0), rect.Max, GetColorU32(c::elements::text), description, ImVec2(0.0, 0.8));
        auto walpha = GetColorU32(ImVec4(c::elements::background.x, c::elements::background.y, c::elements::background.z, 1.f));
        auto nalpha = GetColorU32(ImVec4(c::elements::background.x, c::elements::background.y, c::elements::background.z, 0.f));
        GetWindowDrawList()->AddRectFilled(rect.Min + ImVec2(w - 140, 0), rect.Max, GetColorU32(c::elements::background), c::elements::rounding);
        GetWindowDrawList()->AddRectFilledMultiColor(rect.Min, rect.Max - ImVec2(120, 0),nalpha, walpha, walpha, nalpha);


        const bool value_changed = RangeSliderBehavior(slider_bb, id, v1, v2, v_min, v_max, power, decimal_precision);

        char value_buf[64];
        const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v1, *v2);

        RenderTextColor(font::lexend_bold, rect.Min + ImVec2(0, 7), rect.Max - ImVec2(15, 10), GetColorU32(c::elements::text_active), value_buf, ImVec2(1.0, 0.0f));
        RenderTextColor(font::lexend_bold, rect.Min + ImVec2(10, 0), rect.Max, GetColorU32(c::elements::text_active), label, ImVec2(0.0, 0.2));

        return value_changed;

    }

    struct quand_state
    {
        float slow0, slow1, tickness;
        int move_y, move_x;
        float move_yy, move_xx;
    };

    bool QuandScalar(const char* label, const char* description, ImGuiDataType data_type, void* p_data0, void* p_data1, const void* p_min0, const void* p_max0, const void* p_min1, const void* p_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
    {

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const float w = GetContentRegionMax().x - style.WindowPadding.x;

        ImVec2 pos = window->DC.CursorPos;
        const ImRect frame_bb(pos + ImVec2(10, 55), pos + ImVec2(w - 10, 145));
        const ImRect rect(pos, pos + ImVec2(w, 155));
        const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;

        ItemSize(rect, 0.f);
        if (!ItemAdd(rect, id)) return false;

        if (format0 == NULL) format0 = DataTypeGetInfo(data_type)->PrintFmt;
        if (format1 == NULL) format1 = DataTypeGetInfo(data_type)->PrintFmt;

        bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

        bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);

        ImRect grab_bb0, grab_bb1;

        static std::map<ImGuiID, quand_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, quand_state() });
            it_anim = anim.find(id);
        }

        const bool value_changed0 = SliderBehavior(ImRect(frame_bb.Min - ImVec2(7, 0), frame_bb.Max + ImVec2(5, 0)), id, data_type, p_data0, p_min0, p_max0, format0, flags, &grab_bb0);
        const bool value_changed1 = SliderBehavior(ImRect(frame_bb.Min - ImVec2(0, 7), frame_bb.Max + ImVec2(0, 4)), id, data_type, p_data1, p_max1, p_min1, format1, flags | ImGuiSliderFlags_Vertical, &grab_bb1);

        if (value_changed0) MarkItemEdited(id);

        char value_buf0[64];
        char value_buf1[64];
        const char* value_buf_end0 = value_buf0 + DataTypeFormatString(value_buf0, IM_ARRAYSIZE(value_buf0), data_type, p_data0, format0);
        const char* value_buf_end1 = value_buf1 + DataTypeFormatString(value_buf1, IM_ARRAYSIZE(value_buf1), data_type, p_data1, format1);

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::elements::background), c::elements::rounding);

        it_anim->second.slow0 = ImLerp(it_anim->second.slow0, grab_bb0.Min.x - (frame_bb.Min.x) + 7, g.IO.DeltaTime * 25.f);
        it_anim->second.slow1 = ImLerp(it_anim->second.slow1, grab_bb1.Min.y - (frame_bb.Min.y) + 7, g.IO.DeltaTime * 25.f);

        GetWindowDrawList()->AddLine(ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Min.y), ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Max.y), GetColorU32(c::accent), 0.5f);
        GetWindowDrawList()->AddShadowRect(ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Min.y), ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Max.y), VGetColorU32(c::accent, 0.5f), 25.f, ImVec2(0, 0), 100.f);

        GetWindowDrawList()->AddLine(ImVec2(grab_bb1.Min.x, it_anim->second.slow1 + frame_bb.Min.y), ImVec2(grab_bb1.Max.x, it_anim->second.slow1 + frame_bb.Min.y), GetColorU32(c::accent), 0.5f);
        GetWindowDrawList()->AddShadowRect(ImVec2(grab_bb1.Min.x, it_anim->second.slow1 + frame_bb.Min.y), ImVec2(grab_bb1.Max.x, it_anim->second.slow1 + frame_bb.Min.y), VGetColorU32(c::accent, 0.5f), 25.f, ImVec2(0, 0), 100.f);

        GetWindowDrawList()->AddText(font::lexend_bold, 17, ImVec2(rect.Min.x + 10, rect.Min.y + 10), GetColorU32(c::elements::text_active), label);

        std::string blah0(value_buf0), blah1(value_buf1);

        std::string label_x = "[X, " + blah0 + " Y, " + blah1 + "]";

        GetWindowDrawList()->AddText(font::lexend_bold, 17, ImVec2(rect.Min.x + 10, rect.Min.y + 30), GetColorU32(c::elements::text), description);

        RenderTextColor(font::lexend_bold, rect.Min + ImVec2(0, 7), rect.Max - ImVec2(15, 10), GetColorU32(c::elements::text_active), label_x.c_str(), ImVec2(1.0, 0.0f));

    }

    bool QuandFloat(const char* label, const char* description, float* v0, float* v1, float v_min0, float v_max0, float v_min1, float v_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
    {
        return QuandScalar(label, description, ImGuiDataType_Float, v0, v1, &v_min0, &v_max0, &v_min1, &v_max1, format0, format1, flags);
    }

    bool QuandInt(const char* label, const char* description, int* v0, int* v1, int v_min0, int v_max0, int v_min1, int v_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
    {
        return QuandScalar(label, description, ImGuiDataType_S32, v0, v1, &v_min0, &v_max0, &v_min1, &v_max1, format0, format1, flags);
    }
    bool edited::pointbox(const char* label, bool* v, int type, float x, float y)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        const float checkbox_size = 18.f;
        const ImRect check_bb(ImVec2(window->Pos.x + x, window->Pos.y + y), ImVec2(window->Pos.x + x + checkbox_size, window->Pos.y + y + checkbox_size));
        ItemSize(check_bb, style.FramePadding.y);

        ImRect total_bb = check_bb;

        if (!ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            *v = !(*v);

        // Additional logic for animations
        static std::map<ImGuiID, float> hover_animation;
        auto it_hover = hover_animation.find(id);
        if (it_hover == hover_animation.end())
        {
            hover_animation.insert({ id, 0.f });
            it_hover = hover_animation.find(id);
        }
        it_hover->second = ImClamp(it_hover->second + (0.2f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -5.f)), 0.0f, 0.12f);
        it_hover->second *= g.Style.Alpha;
        static std::map<ImGuiID, float> filled_animation;
        auto it_filled = filled_animation.find(id);
        if (it_filled == filled_animation.end())
        {
            filled_animation.insert({ id, 0.f });
            it_filled = filled_animation.find(id);
        }

        if (*v)
        {
            it_filled->second = ImClamp(it_filled->second + (2.35f * ImGui::GetIO().DeltaTime * 2.f), it_hover->second, 1.f);

        }
        else
        {
            it_filled->second = ImClamp(it_filled->second - (1.5f * ImGui::GetIO().DeltaTime), 0.1f, it_hover->second);
        }

        it_filled->second *= g.Style.Alpha;

        static std::map<ImGuiID, float> circle_amimation;
        auto it_circled = circle_amimation.find(id);
        if (it_circled == circle_amimation.end())
        {
            circle_amimation.insert({ id, 0.f });
            it_circled = circle_amimation.find(id);
        }

        if (*v)
        {
            it_circled->second = ImClamp(it_circled->second + (2.35f * ImGui::GetIO().DeltaTime * 2.f), it_hover->second, 1.f);

        }
        else
        {
            it_circled->second = ImClamp(it_circled->second - (1.5f * ImGui::GetIO().DeltaTime), 1.f, it_hover->second);
        }

        it_circled->second *= g.Style.Alpha;
        const float check_sz = checkbox_size - 2.0f;
        ImVec4 text = ImLerp(ImVec4{ 141 / 255.f, 146 / 255.f, 154 / 255.f, g.Style.Alpha }, ImVec4{ 255 / 255.f, 255 / 255.f, 255 / 255.f, g.Style.Alpha }, it_filled->second);
        // Draw the shadow circle around the checkmark
        ImVec2 center = ImVec2(check_bb.Min.x + check_bb.GetWidth() * 0.5f, check_bb.Min.y + check_bb.GetHeight() * 0.5f);
        float radius = checkbox_size * 0.6f; // Adjust the radius as needed
        float radius2 = checkbox_size * 0.7f; // Adjust the radius as needed

        ImU32 shadow_col = *v ? VGetColorU32(c::accent, (int)(255 * it_filled->second)) : VGetColorU32(c::accent, (int)(155 * it_filled->second)); // Adjust shadow color as needed
        ImU32 circle_col = *v ? ImColor(0, 0, 0, (int)(200 * it_circled->second)) : ImColor(0, 0, 0, (int)(170 * it_circled->second)); // Adjust shadow color as needed

        float shadow_thickness = 32.f; // Adjust shadow thickness as needed
        ImVec2 shadow_offset = ImVec2(0, 0); // Adjust shadow offset as needed
        window->DrawList->AddShadowCircle(center, radius, shadow_col, shadow_thickness, shadow_offset, 0, 12);
        window->DrawList->AddCircleFilled(center, radius, circle_col, 12);

        // Draw the checkbox frames and checkmark
     //   RenderFrame(check_bb.Min, check_bb.Max, ImColor(27, 27, 27, (int)(255 * g.Style.Alpha)), false, 4);
       // RenderFrame(check_bb.Min + ImVec2(1, 1), check_bb.Max - ImVec2(1, 1), ImColor(15, 15, 15, int(255 * GetStyle().Alpha)), false, 4);
       // RenderFrame(check_bb.Min, check_bb.Max, ImColor(19, 148, 250, (int)(255 * it_filled->second)), false, 4);
        const float check_mz = checkbox_size - 5.f;
        if (*v)
            RenderCheckMark(window->DrawList, check_bb.Min + ImVec2((checkbox_size - check_mz) * 0.5f, (checkbox_size - check_mz) * 0.5f), VGetColorU32(c::accent, (int)(255 * it_filled->second)), check_mz);
        else
            RenderCheckMark(window->DrawList, check_bb.Min + ImVec2((checkbox_size - check_mz) * 0.5f, (checkbox_size - check_mz) * 0.5f), ImColor(55, 55, 55, (int)(170 * it_filled->second)), check_mz);


        // Draw the label text

        return pressed;
    }
    bool edited::SmallCheckbox(const char* label, bool* v, int type)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        const float checkbox_size = 18.f;
        const float padding_between = 5.0f;

        float new_y_pos = window->DC.CursorPos.y + 2.f;
        float original_x_pos = type == 0 ? window->Pos.x + style.WindowPadding.x + 10.0f : window->Pos.x + style.WindowPadding.x + 2.f;
        window->DC.CursorPos.x = original_x_pos;

        // Calculate the total bounding box for the checkbox
        const ImRect check_bb(ImVec2(original_x_pos, new_y_pos), ImVec2(original_x_pos + checkbox_size, new_y_pos + checkbox_size));
        ItemSize(check_bb, style.FramePadding.y);

        // Check if the item is interactive and add it
        if (!ItemAdd(check_bb, id))
            return false;

        // Handle button behavior
        bool hovered, held;
        bool pressed = ButtonBehavior(check_bb, id, &hovered, &held);
        if (pressed)
            *v = !(*v);

        // Additional logic for animations
        static std::map<ImGuiID, float> hover_animation;
        auto it_hover = hover_animation.find(id);
        if (it_hover == hover_animation.end())
        {
            hover_animation.insert({ id, 0.f });
            it_hover = hover_animation.find(id);
        }
        it_hover->second = ImClamp(it_hover->second + (0.2f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -5.f)), 0.0f, 0.12f);
        it_hover->second *= g.Style.Alpha;

        static std::map<ImGuiID, float> filled_animation;
        auto it_filled = filled_animation.find(id);
        if (it_filled == filled_animation.end())
        {
            filled_animation.insert({ id, 0.f });
            it_filled = filled_animation.find(id);
        }
        it_filled->second = ImClamp(it_filled->second + (2.35f * ImGui::GetIO().DeltaTime * ((*v) ? 2.f : -2.f)), it_hover->second, 1.f);
        it_filled->second *= g.Style.Alpha;

        const float check_sz = checkbox_size - 2.0f;
        ImVec4 text = ImLerp(ImVec4{ 141 / 255.f, 146 / 255.f, 154 / 255.f, g.Style.Alpha }, ImVec4{ 255 / 255.f, 255 / 255.f, 255 / 255.f, g.Style.Alpha }, it_filled->second);

        // Draw the checkbox frames and checkmark
        RenderFrame(check_bb.Min, check_bb.Max, ImColor(27, 27, 27, (int)(255 * g.Style.Alpha)), false, 4);
        RenderFrame(check_bb.Min + ImVec2(1, 1), check_bb.Max - ImVec2(1, 1), ImColor(15, 15, 15, int(255 * GetStyle().Alpha)), false, 4);
        RenderFrame(check_bb.Min, check_bb.Max, ImColor(19, 148, 250, (int)(255 * it_filled->second)), false, 4);
        const float check_mz = checkbox_size - 5.f;
        if (*v)
            RenderCheckMark(window->DrawList, check_bb.Min + ImVec2((checkbox_size - check_mz) * 0.5f, (checkbox_size - check_mz) * 0.5f), ImColor(255, 255, 255, (int)(255 * it_filled->second)), check_mz);

        // Draw the label text
        const float text_y_pos = new_y_pos + (checkbox_size - GetTextLineHeight()) * 0.5f;
        window->DrawList->AddText(ImVec2(original_x_pos + checkbox_size + padding_between + 4.f, text_y_pos), ImColor(text), label);

        return pressed;
    }
    static void ColorEditRestoreH(const float* col, float* H)
    {
        ImGuiContext& g = *GImGui;
        IM_ASSERT(g.ColorEditCurrentID != 0);
        if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
            return;
        *H = g.ColorEditSavedHue;
    }

    static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
    {
        ImGuiContext& g = *GImGui;
        IM_ASSERT(g.ColorEditCurrentID != 0);
        if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0))) return;

        if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
            *H = g.ColorEditSavedHue;

        if (*V == 0.0f) *S = g.ColorEditSavedSat;
    }

    struct iconbox_state
    {
        float line;
    };

    bool Color(const char* szLabel, const char* szDesc, Color_t * pColor, ImGuiColorEditFlags flags)
    {
       
        float arrColor[4];
        pColor->BaseAlpha(arrColor);

        if (ColorEdit4(szLabel, szDesc, &arrColor[0], flags))
        {
            *pColor = Color_t::FromBase4(arrColor);
            return true;
        }

        return false;
    }

    bool icon_box(const char* icon, ImVec2 size, ImU32 color_bg, ImU32 color_icon, ImU32 color_border)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect rect(pos, pos + size);
        const ImGuiID id = window->GetID(icon);

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        ItemSize(rect, 0.f);
        if (!ItemAdd(rect, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held, NULL);

        static std::map<ImGuiID, iconbox_state> anim;
        iconbox_state& state = anim[id];

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(color_bg), c::elements::rounding);
        GetWindowDrawList()->AddRect(rect.Min, rect.Max, GetColorU32(color_border), c::elements::rounding);

        RenderTextColor(font::icomoon_widget, rect.Min, rect.Max, GetColorU32(color_icon), icon, ImVec2(0.5, 0.5));

        return pressed;
    }

    bool color_button(const char* name, ImVec2 size, ImU32 color_bg)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect rect(pos, pos + size);

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(name);

        ItemSize(rect, 0.f);
        if (!ItemAdd(rect, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held, NULL);

        static std::map<ImGuiID, iconbox_state> anim;
        iconbox_state& state = anim[id];

        GetWindowDrawList()->AddCircleFilled(rect.Min + ImVec2(size / 2), size.x / 2, GetColorU32(color_bg), 30.f);
        GetWindowDrawList()->AddShadowCircle(rect.Min + ImVec2(size / 2), size.x / 2, GetColorU32(color_bg), 18.f, ImVec2(0, 0), 30.f);

        GetWindowDrawList()->AddCircle(rect.Min + ImVec2(size / 2), size.x / 3, VGetColorU32(c::elements::background, 0.3f), 30.f, 3.f);

        return pressed;
    }



    struct edit_state
    {
        ImVec4 text;
        float alpha = 0.f, alpha_search;
        bool hovered, active;
    };

    bool ColorEdit4(const char* label, const char* description, float col[4], ImGuiColorEditFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const float square_sz = GetFrameHeight();
        const float w_full = CalcItemWidth();
        const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
        const float w_inputs = w_full - w_button;
        const char* label_display_end = FindRenderedTextEnd(label);
        g.NextItemData.ClearFlags();

        BeginGroup();
        PushID(label);

        // If we're not showing any slider there's no point in doing any HSV conversions
        const ImGuiColorEditFlags flags_untouched = flags;
        if (flags & ImGuiColorEditFlags_NoInputs)
            flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

        // Context menu: display and modify options (before defaults are applied)
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            ColorEditOptionsPopup(col, flags);

        // Read stored options
        if (!(flags & ImGuiColorEditFlags_DisplayMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
        if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
        if (!(flags & ImGuiColorEditFlags_PickerMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
        if (!(flags & ImGuiColorEditFlags_InputMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
        flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

        const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
        const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
        const int components = alpha ? 4 : 3;

        // Convert to the formats we need
        float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
        if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
        {
            // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
        }
        int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

        bool value_changed = false;
        bool value_changed_as_float = false;

        const ImVec2 pos = window->DC.CursorPos;
        const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
        window->DC.CursorPos.x = pos.x + inputs_offset_x;

        if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
        {
            // RGB/HSV 0..255 Sliders
            const float w_item_one = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
            const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

            const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
            static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
            static const char* fmt_table_int[3][4] =
            {
                {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
                { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
                { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
            };
            static const char* fmt_table_float[3][4] =
            {
                {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
                { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
                { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
            };
            const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

            for (int n = 0; n < components; n++)
            {
                if (n > 0)
                    SameLine(0, style.ItemInnerSpacing.x);
                SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

                // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
                if (flags & ImGuiColorEditFlags_Float)
                {
                    value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                    value_changed_as_float |= value_changed;
                }
                else
                {
                    value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
                }
                if (!(flags & ImGuiColorEditFlags_NoOptions))
                    OpenPopupOnItemClick("context");
            }
        }
        else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
        {
            // RGB Hexadecimal Input
            char buf[64];
            if (alpha)
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
            else
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
            SetNextItemWidth(w_inputs);
            if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
            {
                value_changed = true;
                char* p = buf;
                while (*p == '#' || ImCharIsBlankA(*p))
                    p++;
                i[0] = i[1] = i[2] = 0;
                i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
                int r;
                if (alpha)
                    r = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
                else
                    r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
                IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context");
        }

        ImGuiWindow* picker_active_window = NULL;
        if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
        {
            const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
            window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

            const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
            if (ColorButton("##ColorButton", col_v4, flags))
            {
                if (!(flags & ImGuiColorEditFlags_NoPicker))
                {
                    // Store current color and open a picker
                    g.ColorPickerRef = col_v4;
                    OpenPopup("picker");
                    SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
                }
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context");

            if (BeginPopup("picker"))
            {
                picker_active_window = g.CurrentWindow;
                if (label != label_display_end)
                {
                    TextEx(label, label_display_end);
                    Spacing();
                }
                ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
                SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
                value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
                EndPopup();
            }
        }

        if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
        {
            const float text_offset_x = (flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x;
            window->DC.CursorPos = ImVec2(pos.x + text_offset_x, pos.y + style.FramePadding.y);
            TextEx(label, label_display_end);
        }

        // Convert back
        if (value_changed && picker_active_window == NULL)
        {
            if (!value_changed_as_float)
                for (int n = 0; n < 4; n++)
                    f[n] = i[n] / 255.0f;
            if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
            {
                g.ColorEditLastHue = f[0];
                g.ColorEditLastSat = f[1];
                ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
                g.ColorEditLastColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
            }
            if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

            col[0] = f[0];
            col[1] = f[1];
            col[2] = f[2];
            if (alpha)
                col[3] = f[3];
        }

        PopID();
        EndGroup();

        // Drag and Drop Target
        // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
        if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
        {
            bool accepted_drag_drop = false;
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512
                value_changed = accepted_drag_drop = true;
            }
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * components);
                value_changed = accepted_drag_drop = true;
            }

            // Drag-drop payloads are always RGB
            if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
            EndDragDropTarget();
        }

        // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
        if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
            g.LastItemData.ID = g.ActiveId;

        if (value_changed)
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

    static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha)
    {
        ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x + 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32(0, 0, 0, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x, pos.y), half_sz, ImGuiDir_Right, IM_COL32(255, 255, 255, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left, IM_COL32(0, 0, 0, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x, pos.y), half_sz, ImGuiDir_Left, IM_COL32(255, 255, 255, alpha8));
    }

    struct picker_state
    {
        float hue_bar;
        float alpha_bar;
        float circle;
        ImVec2 circle_move;
    };

    bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImDrawList* draw_list = window->DrawList;
        ImGuiStyle& style = g.Style;
        ImGuiIO& io = g.IO;

        const float width = CalcItemWidth();
        g.NextItemData.ClearFlags();

        PushID(label);
        const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
        if (set_current_color_edit_id)
            g.ColorEditCurrentID = window->IDStack.back();
        BeginGroup();

        if (!(flags & ImGuiColorEditFlags_NoSidePreview)) flags |= ImGuiColorEditFlags_NoSmallPreview;

        if (!(flags & ImGuiColorEditFlags_NoOptions)) ColorPickerOptionsPopup(col, flags);

        if (!(flags & ImGuiColorEditFlags_PickerMask_)) flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
        if (!(flags & ImGuiColorEditFlags_InputMask_)) flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));
        if (!(flags & ImGuiColorEditFlags_NoOptions)) flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

        int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
        bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
        ImVec2 picker_pos = window->DC.CursorPos;
        float square_sz = GetFrameHeight();
        float bars_width = 15.f;
        float sv_picker_size = ImMax(bars_width * 1, width - (alpha_bar ? 2 : 1) * (bars_width));
        float bar0_pos_x = picker_pos.x + sv_picker_size + 10;
        float bar1_pos_x = bar0_pos_x + bars_width + 10;
        float bars_triangles_half_sz = IM_FLOOR(bars_width * 0.20f);

        float backup_initial_col[4];
        memcpy(backup_initial_col, col, components * sizeof(float));

        float H = col[0], S = col[1], V = col[2];
        float R = col[0], G = col[1], B = col[2];
        if (flags & ImGuiColorEditFlags_InputRGB)
        {
            ColorConvertRGBtoHSV(R, G, B, H, S, V);
            ColorEditRestoreHS(col, &H, &S, &V);
        }
        else if (flags & ImGuiColorEditFlags_InputHSV)
        {
            ColorConvertHSVtoRGB(H, S, V, R, G, B);
        }

        bool value_changed = false, value_changed_h = false, value_changed_sv = false;

        InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
        if (IsItemActive())
        {
            S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
            V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
            ColorEditRestoreH(col, &H);
            value_changed = value_changed_sv = true;
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions)) OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

        SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
        InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
        if (IsItemActive())
        {
            H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
            value_changed = value_changed_h = true;
        }

        if (alpha_bar)
        {
            SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
            InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
            if (IsItemActive())
            {
                col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
                value_changed = true;
            }
        }

        if (!(flags & ImGuiColorEditFlags_NoLabel))
        {
            const char* label_display_end = FindRenderedTextEnd(label);
            if (label != label_display_end)
            {
                if ((flags & ImGuiColorEditFlags_NoSidePreview))
                    SameLine(0, style.ItemInnerSpacing.x);
                TextEx(label, label_display_end);
            }
        }

        if (value_changed_h || value_changed_sv)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
                g.ColorEditSavedHue = H;
                g.ColorEditSavedSat = S;
                g.ColorEditSavedID = g.ColorEditCurrentID;
                g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
            }
            else if (flags & ImGuiColorEditFlags_InputHSV)
            {
                col[0] = H;
                col[1] = S;
                col[2] = V;
            }
        }

        if (value_changed)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                R = col[0];
                G = col[1];
                B = col[2];
                ColorConvertRGBtoHSV(R, G, B, H, S, V);
                ColorEditRestoreHS(col, &H, &S, &V);
            }
            else if (flags & ImGuiColorEditFlags_InputHSV)
            {
                H = col[0];
                S = col[1];
                V = col[2];
                ColorConvertHSVtoRGB(H, S, V, R, G, B);
            }
        }

        const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
        const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
        const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
        const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
        const ImU32 col_hues[6 + 1] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(0,255,255,style_alpha8), IM_COL32(0,0,255,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

        ImVec4 hue_color_f(1, 1, 1, style.Alpha); ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
        ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
        ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha));

        static std::map<ImGuiID, picker_state> anim;
        picker_state& state = anim[ImGui::GetID(label)];

        ImVec2 sv_cursor_pos;

        draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), col_white, hue_color32, hue_color32, col_white);
        draw_list->AddRectFilledMultiColor(picker_pos - ImVec2(1, 1), picker_pos + ImVec2(sv_picker_size + 1, sv_picker_size + 1), 0, 0, col_black, col_black);

        sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S) * sv_picker_size), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2);
        sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * sv_picker_size), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);

        for (int i = 0; i < 6; ++i) draw_list->AddRectFilledMultiColor(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6) - (i == 5 ? 1 : 0)), ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6) + (i == 0 ? 1 : 0)), col_hues[i], col_hues[i], col_hues[i + 1], col_hues[i + 1]);

        float bar0_line_y = IM_ROUND(picker_pos.y + H * sv_picker_size);
        bar0_line_y = ImClamp(bar0_line_y, picker_pos.y + 3.f, picker_pos.y + (sv_picker_size - 13));

        state.hue_bar = ImLerp(state.hue_bar, bar0_line_y + 5, g.IO.DeltaTime * 24.f);
        draw_list->AddCircleFilled(ImVec2(bar0_pos_x + 7.5f, state.hue_bar), 4.5f, col_black, 100.f);

        float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
        int sv_cursor_segments = draw_list->_CalcCircleAutoSegmentCount(sv_cursor_rad);

        state.circle_move = ImLerp(state.circle_move, sv_cursor_pos, g.IO.DeltaTime * 10.f);
        state.circle = ImLerp(state.circle, value_changed_sv ? 6.0f : 4.0f, g.IO.DeltaTime * 24.f);

        draw_list->AddCircle(state.circle_move, state.circle, col_white, sv_cursor_segments, 2.f);

        if (alpha_bar)
        {
            float alpha = ImSaturate(col[3]);
            ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);

            draw_list->AddRectFilledMultiColor(bar1_bb.Min, bar1_bb.Max, user_col32_striped_of_alpha, user_col32_striped_of_alpha, user_col32_striped_of_alpha & ~IM_COL32_A_MASK, user_col32_striped_of_alpha & ~IM_COL32_A_MASK);

            float bar1_line_y = IM_ROUND(picker_pos.y + (1.0f - alpha) * sv_picker_size);
            bar1_line_y = ImClamp(bar1_line_y, picker_pos.y + 3.f, picker_pos.y + (sv_picker_size - 13));

            state.alpha_bar = ImLerp(state.alpha_bar, bar1_line_y + 5, g.IO.DeltaTime * 24.f);
            draw_list->AddCircleFilled(ImVec2(bar1_pos_x + 7.5f, state.alpha_bar), 4.5f, col_black, 100.f);
        }

        EndGroup();

        if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0) value_changed = false;
        if (value_changed) MarkItemEdited(g.LastItemData.ID);

        PopID();

        return value_changed;
    }



    bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, ImVec2 size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID id = window->GetID(desc_id);
        float default_size = GetFrameHeight();
        if (size_arg.x == 0.0f)
            size_arg.x = default_size;
        if (size_arg.y == 0.0f)
            size_arg.y = default_size;
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size_arg);
        ItemSize(bb, (size_arg.y >= default_size) ? g.Style.FramePadding.y : 0.0f);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        if (flags & ImGuiColorEditFlags_NoAlpha)
            flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

        ImVec4 col_rgb = col;
        if (flags & ImGuiColorEditFlags_InputHSV)
            ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

        ImVec4 col_rgb_without_alpha(col_rgb.x, col_rgb.y, col_rgb.z, 1.0f);
        float grid_step = ImMin(size_arg.x, size_arg.y) / 2.99f;
        float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
        ImRect bb_inner = bb;
        float off = 0.0f;
        if ((flags & ImGuiColorEditFlags_NoBorder) == 0)
        {
            off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
            bb_inner.Expand(off);
        }
        if ((flags & ImGuiColorEditFlags_AlphaPreviewHalf) && col_rgb.w < 1.0f)
        {
            float mid_x = IM_ROUND((bb_inner.Min.x + bb_inner.Max.x) * 0.5f);
            RenderColorRectWithAlphaCheckerboard(window->DrawList, ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, GetColorU32(col_rgb), grid_step, ImVec2(-grid_step + off, off), rounding, ImDrawFlags_RoundCornersRight);
            window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), GetColorU32(col_rgb_without_alpha), rounding, ImDrawFlags_RoundCornersLeft);
        }
        else
        {
            // Because GetColorU32() multiplies by the global style Alpha and we don't want to display a checkerboard if the source code had no alpha
            ImVec4 col_source = (flags & ImGuiColorEditFlags_AlphaPreview) ? col_rgb : col_rgb_without_alpha;
            if (col_source.w < 1.0f)
                RenderColorRectWithAlphaCheckerboard(window->DrawList, bb_inner.Min, bb_inner.Max, GetColorU32(col_source), grid_step, ImVec2(off, off), rounding);
            else
                window->DrawList->AddRectFilled(bb_inner.Min, bb_inner.Max, GetColorU32(col_source), rounding);
        }
        RenderNavHighlight(bb, id);
        if ((flags & ImGuiColorEditFlags_NoBorder) == 0)
        {
            if (g.Style.FrameBorderSize > 0.0f)
                RenderFrameBorder(bb.Min, bb.Max, rounding);
            else
                window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), rounding); // Color button are often in need of some sort of border
        }

        // Drag and Drop Source
        // NB: The ActiveId test is merely an optional micro-optimization, BeginDragDropSource() does the same test.
        if (g.ActiveId == id && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropSource())
        {
            if (flags & ImGuiColorEditFlags_NoAlpha)
                SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F, &col_rgb, sizeof(float) * 3, ImGuiCond_Once);
            else
                SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F, &col_rgb, sizeof(float) * 4, ImGuiCond_Once);
            ColorButton(desc_id, col, flags);
            SameLine();
            TextEx("Color");
            EndDragDropSource();
        }

        // Tooltip
        if (!(flags & ImGuiColorEditFlags_NoTooltip) && hovered)
            ColorTooltip(desc_id, &col.x, flags & (ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf));

        return pressed;
    }



    struct preview_state
    {
        ImVec4 box_color, nick_color, weapon_color, zoom_color, bomb_color, c4_color, money_color, hit_color, hp_color, hp_line_color;
        ImVec2 nick_pos;
    };

    struct OverlayTextElement {
        bool* condition;
        ImVec2 position_offset;
        ImU32 color;
        std::string text;
        ImVec2 measurement_offset;
    };

    void esp_preview(ImTextureID player_preview, bool* nickname, float nick_color[4], bool* weapon, float weapon_color[4], int* hp, float hp_color[4], bool* zoom, float zoom_color[4], bool* bomb, float bomb_color[4], bool* c4, float c4_color[4], bool* money, float money_color[4], bool* hit, float hit_color[4], bool* esp_box, float box_color[4], bool* hp_line, float hp_line_color[4])
    {
        ImGuiWindow* window = GetCurrentWindow();
        ImGuiContext& g = *GImGui;

        const ImGuiID id = window->GetID(player_preview);
        const ImVec2 pos = window->DC.CursorPos;
        const float width = GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x;
        const ImRect player(pos + ImVec2((width - 118) / 2, 30), pos + ImVec2((width + 118) / 2, 313));
        const ImRect box(pos + ImVec2((width - 210) / 2, 20), pos + ImVec2((width + 210) / 2, 327));

        ItemSize(ImRect(box.Min + ImVec2(0, 0), box.Max + ImVec2(0, 45)));

        static std::map<ImGuiID, preview_state> anim;
        preview_state& state = anim[id];

        state.hp_line_color = ImLerp(state.hp_line_color, ImColor(hp_line_color[0], hp_line_color[1], hp_line_color[2], hp_line_color[3]), g.IO.DeltaTime * 6.f);
        state.weapon_color = ImLerp(state.weapon_color, ImColor(weapon_color[0], weapon_color[1], weapon_color[2], weapon_color[3]), g.IO.DeltaTime * 6.f);
        state.money_color = ImLerp(state.money_color, ImColor(money_color[0], money_color[1], money_color[2], money_color[3]), g.IO.DeltaTime * 6.f);
        state.bomb_color = ImLerp(state.bomb_color, ImColor(bomb_color[0], bomb_color[1], bomb_color[2], bomb_color[3]), g.IO.DeltaTime * 6.f);
        state.zoom_color = ImLerp(state.zoom_color, ImColor(zoom_color[0], zoom_color[1], zoom_color[2], zoom_color[3]), g.IO.DeltaTime * 6.f);
        state.nick_color = ImLerp(state.nick_color, ImColor(nick_color[0], nick_color[1], nick_color[2], nick_color[3]), g.IO.DeltaTime * 6.f);
        state.hit_color = ImLerp(state.hit_color, ImColor(hit_color[0], hit_color[1], hit_color[2], hit_color[3]), g.IO.DeltaTime * 6.f);
        state.box_color = ImLerp(state.box_color, ImColor(box_color[0], box_color[1], box_color[2], box_color[3]), g.IO.DeltaTime * 6.f);
        state.hp_color = ImLerp(state.hp_color, ImColor(hp_color[0], hp_color[1], hp_color[2], hp_color[3]), g.IO.DeltaTime * 6.f);
        state.c4_color = ImLerp(state.c4_color, ImColor(c4_color[0], c4_color[1], c4_color[2], c4_color[3]), g.IO.DeltaTime * 6.f);

        int hp_result = 307 * *hp / 100;

        GetWindowDrawList()->AddImage(player_preview, player.Min, player.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(c::elements::text_active));

        PushFont(font::lexend_bold);

        if (*esp_box)  GetWindowDrawList()->AddRect(box.Min, box.Max, GetColorU32(state.box_color), 0.f, NULL, 2.f);

        if (*nickname) GetWindowDrawList()->AddText(pos + ImVec2((width - CalcTextSize("Nickname").x) / 2, 0), GetColorU32(state.nick_color), "Nickname");

        if (*money)    GetWindowDrawList()->AddText(box.Max - ImVec2(-7, 310), GetColorU32(state.money_color), "50$");

        PopFont();
    }
}