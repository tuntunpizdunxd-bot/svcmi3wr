#include "imgui.h"

inline float dpi = 1.0f;

namespace font
{
	inline ImFont* icomoon = nullptr;
	inline ImFont* skeet = nullptr;
	inline ImFont* lexend_bold = nullptr;
	inline ImFont* lexend_regular = nullptr;
    inline ImFont* intro = nullptr;
	inline ImFont* lexend_general_bold = nullptr;


    inline ImFont* Icons = nullptr;
    inline ImFont* VisIcons = nullptr;

	inline ImFont* icomoon_widget = nullptr;
	inline ImFont* icomoon_widget2 = nullptr;

}

namespace c
{

	inline ImVec4 accent = ImColor(196, 55, 64);

	namespace background
	{

		inline ImVec4 filling = ImColor(12, 12, 12);
		inline ImVec4 stroke = ImColor(24, 26, 36);
		inline ImVec2 size = ImVec2(800, 715);

		inline float rounding = 50;

	}

	namespace elements
	{
		inline ImVec4 mark = ImColor(255, 255, 255);

		inline ImVec4 stroke = ImColor(28, 26, 37);
		inline ImVec4 background = ImColor(15, 15, 17);
		inline ImVec4 background_widget = ImColor(21, 23, 26);

		inline ImVec4 text_active = ImColor(255, 255, 255);
		inline ImVec4 text_hov = ImColor(81, 92, 109);
		inline ImVec4 text = ImColor(43, 51, 63);

		inline float rounding = 4;
	}

	namespace child
	{
        inline ImVec4 general_color = ImColor(195, 105, 48, 255);
        inline ImVec4 main_color = ImColor(16, 16, 16, 255);
        inline ImVec4 lite_color = ImColor(20, 20, 20, 255);
        inline ImVec4 gray_color = ImColor(43, 43, 43, 255);
        inline ImVec4 Tab_Child = ImColor(15, 15, 15, 255);
        inline ImVec4 Tab_Border = ImColor(26, 26, 26, 255);

        inline ImVec4 Tab_Selected = ImColor(196, 55, 64);
        inline ImVec4 Tab_Hovered = ImColor(130, 130, 130, 255);
        inline ImVec4 Tab = ImColor(80, 80, 80, 255);

        inline ImVec4 Checkbox = ImColor(14, 14, 14, 255);
        inline ImVec4 Checkbox_Hovered = ImColor(12, 12, 12, 255);
        inline ImVec4 Checkbox_Active = ImColor(218, 96, 21, 255);

        inline ImVec4 Car_Slider = ImColor(221, 97, 23, 255);
        inline ImVec4 Car_Slider_Hovered = ImColor(196, 55, 64);
        inline ImVec4 Car_Slider_Active = ImColor(196, 55, 64);

        inline ImVec4 Slider = ImColor(16, 16, 16, 255);
        inline ImVec4 Slider_Hovered = ImColor(18, 18, 18, 255);
        inline ImVec4 Slider_Active = ImColor(18, 18, 18, 255);

        inline ImVec4 Circle_Slider = ImColor(255, 255, 255, 255);
        inline ImVec4 Circle_SliderHovered = ImColor(255, 255, 255, 255);
        inline ImVec4 Circle_SliderActive = ImColor(255, 255, 255, 255);

        inline ImVec4 Combo = ImColor(26, 26, 26, 180);
        inline ImVec4 Combo_Hovered = ImColor(26, 26, 26, 180);
        inline ImVec4 Combo_Active = ImColor(26, 26, 26, 180);

        inline ImVec4 InputText = ImColor(16, 16, 16, 150);
        inline ImVec4 InputText_Hovered = ImColor(18, 18, 18, 150);
        inline ImVec4 InputText_Active = ImColor(18, 18, 18, 255);

        inline ImVec4 Button = ImColor(26, 26, 26, 150);
        inline ImVec4 Button_Hovered = ImColor(30, 30, 30, 255);
        inline ImVec4 Button_Active = ImColor(33, 33, 33, 255);

        inline ImVec4 Selectable_Hovered = ImColor(150, 150, 150, 255);
        inline ImVec4 Selectable_Active = ImColor(250, 250, 250, 255);

        inline ImVec4 Picker_Active = ImColor(20, 20, 20, 255);

        inline ImVec4 Text = ImColor(71, 71, 71, 255);
        inline ImVec4 Text_Hovered = ImColor(200, 200, 200, 255);
        inline ImVec4 Text_Active = ImColor(235, 245, 255, 255);

        inline ImVec4 CheckMark = ImColor(255, 255, 255, 255);

        inline ImVec4 Transparent = ImColor(0, 0, 0, 0);

        inline ImVec4 tab_tooltip = ImColor(180, 180, 180, 255);

        inline ImVec4 keybind_background = ImColor(17, 17, 17, 255);
        inline ImVec4 keybind_border = ImColor(28, 28, 28, 255);
	}

	namespace tab
	{
		inline ImVec4 tab_active = ImColor(22, 22, 30);

		inline ImVec4 border = ImColor(14, 14, 15);
	}

}