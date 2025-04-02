#include "Keys.h"
#include <spdlog/spdlog.h>

Keys* Keys::getInstance() {
    static Keys instance;  // Guaranteed to be created only once
    return &instance;  // Return raw pointer to the instance
}

void Keys::handleInput(SDL_Event& gEvent) {
    if (gEvent.type == SDL_KEYDOWN)
        spdlog::info("Key Pressed: {}", gEvent.key.keysym.sym);
}