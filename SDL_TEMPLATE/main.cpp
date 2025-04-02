#include <SDL.h>
#include "Game.h"

int SDL_main(int argc, char* argv[]) {
    Game* game = Game::getInstance();

    game->init();

    while (game->isRunning()) {
        game->input();
        game->update();
        game->render();
    }

    game->clean();

    return 0;
}