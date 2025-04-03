#include "ImGuiGamePanel.h"
#include "spdlog/spdlog.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
#include "SDL.h"
#include "ObjectInfo.h"

ImGuiGamePanel* ImGuiGamePanel::getInstance() {
    static ImGuiGamePanel instance;
    return &instance;
}

void ImGuiGamePanel::initImGui(SDL_Window* window, SDL_GLContext& context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    if (!ImGui::GetCurrentContext()) {
        spdlog::error("Error: ImGui context creation failed!");
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL2_InitForOpenGL(window, context)) {
        spdlog::error("Error: Failed to initialize ImGui for SDL2.");
        return;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 430")) {
        spdlog::error("Error: Failed to initialize ImGui for OpenGL.");
        return;
    }

    spdlog::info("ImGui successfully initialized with SDL2 and OpenGL!");
}

void ImGuiGamePanel::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    ImGui::Begin("Debug Window");

    static ImVec4 color = ImVec4(ObjectInfo::bgR, ObjectInfo::bgG, ObjectInfo::bgB, ObjectInfo::bgA);
    static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;
    ImGui::SeparatorText("Color picker");

    static bool ref_color = false;
    static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
    static int picker_mode = 0;
    static int display_mode = 0;
    static ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_AlphaBar;

    ImGui::CheckboxFlags("ImGuiColorEditFlags_NoAlpha", &color_picker_flags, ImGuiColorEditFlags_NoAlpha);
    ImGui::CheckboxFlags("ImGuiColorEditFlags_AlphaBar", &color_picker_flags, ImGuiColorEditFlags_AlphaBar);
    ImGui::CheckboxFlags("ImGuiColorEditFlags_NoSidePreview", &color_picker_flags, ImGuiColorEditFlags_NoSidePreview);
    if (color_picker_flags & ImGuiColorEditFlags_NoSidePreview) {
        ImGui::SameLine();
        ImGui::Checkbox("With Ref Color", &ref_color);
        if (ref_color) {
            ImGui::SameLine();
            ImGui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | base_flags);
        }
    }

    ImGui::Combo("Picker Mode", &picker_mode, "Auto/Current\0ImGuiColorEditFlags_PickerHueBar\0ImGuiColorEditFlags_PickerHueWheel\0");

    ImGui::Combo("Display Mode", &display_mode, "Auto/Current\0ImGuiColorEditFlags_NoInputs\0ImGuiColorEditFlags_DisplayRGB\0ImGuiColorEditFlags_DisplayHSV\0ImGuiColorEditFlags_DisplayHex\0");

    ImGuiColorEditFlags flags_1 = base_flags | color_picker_flags;
    if (picker_mode == 1)  flags_1 |= ImGuiColorEditFlags_PickerHueBar;
    if (picker_mode == 2)  flags_1 |= ImGuiColorEditFlags_PickerHueWheel;
    if (display_mode == 1) flags_1 |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
    if (display_mode == 2) flags_1 |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
    if (display_mode == 3) flags_1 |= ImGuiColorEditFlags_DisplayHSV;
    if (display_mode == 4) flags_1 |= ImGuiColorEditFlags_DisplayHex;
    ImGui::ColorPicker4("MyColor##4", (float*)&color, flags_1);

    ObjectInfo::bgR = color.x;
    ObjectInfo::bgG = color.y;
    ObjectInfo::bgB = color.z;
    ObjectInfo::bgA = color.w;

    ImGui::Text("Set defaults in code:");
    if (ImGui::Button("Default: Uint8 + HSV + Hue Bar"))
        ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_PickerHueBar);
    if (ImGui::Button("Default: Float + HDR + Hue Wheel"))
        ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);


    ImGui::SeparatorText("X Position:");
    static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    ImGui::SliderFloat("SliderFloat (-1 -> 1)", &ObjectInfo::xPos, -1.0f, 1.0f, "%.3f");

    ImGui::SeparatorText("Mouse Events:");
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsMousePosValid())
        ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
    else
        ImGui::Text("Mouse pos: <INVALID>");
    ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
    ImGui::Text("Mouse down:");
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
    ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
    ImGui::Text("Mouse clicked count:");
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseClickedCount[i] > 0) { ImGui::SameLine(); ImGui::Text("b%d: %d", i, io.MouseClickedCount[i]); }

    // We iterate both legacy native range and named ImGuiKey ranges. This is a little unusual/odd but this allows
    // displaying the data for old/new backends.
    // User code should never have to go through such hoops!
    // You can generally iterate between ImGuiKey_NamedKey_BEGIN and ImGuiKey_NamedKey_END.
    struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
    ImGuiKey start_key = ImGuiKey_NamedKey_BEGIN;
    ImGui::Text("Keys down:");         for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) continue; ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key); }
    ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
    ImGui::Text("Chars queue:");       for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.
    ImGui::End();

    ImGui::ShowDemoWindow();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}
