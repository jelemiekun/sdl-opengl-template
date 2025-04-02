#include "Window.h"
#include "spdlog/spdlog.h"
#include "Game.h"

Window::Window() : mWindow(nullptr), mWindowID(-1),
mWidth(0), mHeight(0), mMouseFocus(false), mKeyboardFocus(false),
mFullScreen(false), mMinimized(false), mShown(false) {
}

Window::~Window() {
    free();
}

bool Window::init() {
    const int width = 640;
    const int height = 480;

    mWindow = SDL_CreateWindow(
        "SDL Tutorial", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!mWindow) {
        spdlog::error("Window creation failed: {}", SDL_GetError());
        return false;
    } else {
        spdlog::info("Window {} creation success.", SDL_GetWindowID(mWindow));
    }

    mMouseFocus = mKeyboardFocus = true;
    mShown = true;
    mWidth = width;
    mHeight = height;
    mWindowID = SDL_GetWindowID(mWindow);

    return true;
}

void Window::handleEvent(SDL_Event& e) {
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
        switch (e.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            mShown = true;
            break;

        case SDL_WINDOWEVENT_HIDDEN:
            mShown = false;
            break;

        case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            break;

        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(mWindow);
            mShown = false;
            break;
        }
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11) {
        toggleFullscreen();
    }

    if (e.type == SDL_QUIT) {
        if (mWindowID == 1) {
            Game* game = Game::getInstance();
            game->quit();
        }
    }
}

void Window::focus() {
    if (!mShown) {
        SDL_ShowWindow(mWindow);
    }
    SDL_RaiseWindow(mWindow);
}

void Window::render() {
    if (!mMinimized) {
        // TODO
    }
}

void Window::free() {
    if (mWindow) {
        spdlog::info("Freeing window...");
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    mWidth = mHeight = 0;
}

void Window::toggleFullscreen() {
    if (mFullScreen) {
        SDL_SetWindowFullscreen(mWindow, 0);
        mFullScreen = false;
    } else {
        SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        mFullScreen = true;
    }
}

SDL_Window* Window::getWindow() { return mWindow; }
int Window::getWidth() { return mWidth; }
int Window::getHeight() { return mHeight; }
bool Window::hasMouseFocus() { return mMouseFocus; }
bool Window::hasKeyboardFocus() { return mKeyboardFocus; }
bool Window::isMinimized() { return mMinimized; }
bool Window::isShown() { return mShown; }