#pragma once
#include <SDL.h>

class Window {
public:
	Window();
	~Window();

private:
    SDL_Window* mWindow;
    int mWindowID;
    int mWidth, mHeight;

    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
    bool mShown;

public:
	bool init();
    void handleEvent(SDL_Event& e);
    void focus();
    void render();
    void free();
    void toggleFullscreen();

    SDL_Window* getWindow();
    int getWidth();
    int getHeight();
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    bool isShown();
};

