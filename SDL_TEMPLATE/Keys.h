#pragma once
#include "SDL.h"

class Keys {
private:
    Keys() {}

public:
    Keys(const Keys&) = delete;
    Keys& operator=(const Keys&) = delete;
    Keys(Keys&&) = delete;
    Keys& operator=(Keys&&) = delete;

    static Keys* getInstance();

    void handleInput(SDL_Event& gEvent);
};

