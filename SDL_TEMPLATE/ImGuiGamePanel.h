#pragma once
#include <SDL.h>

class ImGuiGamePanel {
private:
    ImGuiGamePanel() {}

public:
    ImGuiGamePanel(const ImGuiGamePanel&) = delete;
    ImGuiGamePanel& operator=(const ImGuiGamePanel&) = delete;
    ImGuiGamePanel(ImGuiGamePanel&&) = delete;
    ImGuiGamePanel& operator=(ImGuiGamePanel&&) = delete;

    static ImGuiGamePanel* getInstance();

    void initImGui(SDL_Window* window, SDL_GLContext& context);
    void render();
};