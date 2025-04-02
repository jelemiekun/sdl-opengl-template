#include "Game.h"
#include <SDL.h>
#include <glad/glad.h>
#include "Window.h"
#include "spdlog/spdlog.h"
#include "Shader.h"
#include "Keys.h"
#include "ElementBuffer.h"
#include "VertexBuffer.h"

Game::Game() : running(false), gWindow(nullptr), glContext(nullptr), 
    gShader(nullptr), vertexBuffer(nullptr), elementBuffer(nullptr) {}

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

void Game::genVertexArrayObject() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void Game::setupShader() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gShader = std::make_unique<Shader>("source.shader");

    std::vector<GLfloat> vertices = {
        -0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 2, 3
    };

    vertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(GLfloat));
    elementBuffer = std::make_unique<ElementBuffer>(indices.data(), indices.size() * sizeof(unsigned int));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Game::init() {
    bool initFailed = (!initSDL() || !initOpenGL());

    if (initFailed) {
        spdlog::warn("Failed to initialize game.");
    } else {
        spdlog::info("Game initialization success.");
        running = true;
    }

    genVertexArrayObject();
    setupShader();
}

void Game::input() {
    static Keys* keys = Keys::getInstance();

    while (SDL_PollEvent(&gEvent)) {
        gWindow.get()->handleEvent(gEvent);
        keys->handleInput(gEvent);
    }
}

void Game::update() {
    glViewport(0, 0, gWindow.get()->getWidth(), gWindow.get()->getHeight());
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(VAO);
    gShader.get()->bind();

    glDrawElements(GL_TRIANGLES, elementBuffer.get()->getCount(), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(gWindow.get()->getWindow());
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