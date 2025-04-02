#include "Game.h"
#include <SDL.h>
#include <glad/glad.h>
#include "Window.h"
#include "spdlog/spdlog.h"
#include "Shader.h"

Game::Game() : running(false), gWindow(nullptr), glContext(nullptr), gShader(nullptr) {}

Game* Game::getInstance() {
    static Game instance;
    return &instance;
}

bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
        return false;
    } else {
        spdlog::info("SDL initialization success.");
        setOpenGLAttributes();
        return initSDLWindow();
    }
}

bool Game::initSDLWindow() {
    gWindow = std::make_unique<Window>();

    gWindow.get()->init();

    if (gWindow) {
        spdlog::info("SDL Window creation success.");
        return true;
    } else {
        spdlog::error("SDL Window Failed to create: {}", SDL_GetError());
        return false;
    }
}

void Game::setOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

bool Game::initOpenGL() {
    glContext = SDL_GL_CreateContext(gWindow.get()->getWindow());

    if (!glContext) {
        spdlog::error("SDL_GL_CreateContext failed: {}", SDL_GetError());
        return false;
    } else {
        spdlog::info("SPD_GL_CreateContext success.");
        return initGlad();
    }
}

bool Game::initGlad() {
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        spdlog::error("Failed to initialize GLAD.");
        return false;
    } else {
        spdlog::info("Glad initialization success.");
        return true;
    }
}

void Game::setupShader() {
    gShader = std::make_unique<Shader>("source.shader");
}

void Game::init() {
    bool initFailed = (!initSDL() || !initOpenGL());

    if (initFailed) {
        spdlog::warn("Failed to initialize game.");
    } else {
        spdlog::info("Game initialization success.");
        running = true;
    }

    setupShader();
}

void Game::input() {
    while (SDL_PollEvent(&gEvent)) {
        gWindow.get()->handleEvent(gEvent);
    }
}

void Game::update() {

}

void Game::render() {
    
}

void Game::quit() {
    running = false;
}

const bool& Game::isRunning() const {
    return running;
}

void Game::clean() {
    gWindow.get()->free();
    spdlog::info("Freeing SDL openGL context...");
    SDL_GL_DeleteContext(glContext);
    spdlog::info("Quitting SDL...");
    SDL_Quit();
}