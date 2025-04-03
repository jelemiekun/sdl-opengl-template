#include "Game.h"
#include <SDL.h>
#include <glad/glad.h>
#include "Window.h"
#include "spdlog/spdlog.h"
#include "Shader.h"
#include "Keys.h"
#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "imgui/imgui_impl_sdl2.h"
#include "ImGuiGamePanel.h"
#include "ObjectInfo.h"

Game::Game() : running(false), gWindow(nullptr), glContext(nullptr), 
    gShader1(nullptr), vertexBuffer1(nullptr), elementBuffer1(nullptr),
    gShader2(nullptr), vertexBuffer2(nullptr), elementBuffer2(nullptr),
    imGuiGamePanel(nullptr) {}

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
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);
}

void Game::setupShader() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glBindVertexArray(VAO1);
    gShader1 = std::make_unique<Shader>("source1.shader");

    std::vector<GLfloat> vertices1 = {
        ObjectInfo::xPos, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        ObjectInfo::xPos, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        ObjectInfo::xPos + 0.2f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        ObjectInfo::xPos + 0.2f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices1 = {
        0, 1, 2,
        1, 2, 3
    };

    vertexBuffer1 = std::make_unique<VertexBuffer>(vertices1.data(), vertices1.size() * sizeof(GLfloat));
    elementBuffer1 = std::make_unique<ElementBuffer>(indices1.data(), indices1.size() * sizeof(unsigned int));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO2);
    gShader2 = std::make_unique<Shader>("source2.shader");

    std::vector<GLfloat> vertices2 = {
        -1.0f, 1.0f, 0.0f, ObjectInfo::bgR, ObjectInfo::bgG, ObjectInfo::bgB,
        -1.0f, -1.0f, 0.0f, ObjectInfo::bgR, ObjectInfo::bgG, ObjectInfo::bgB,
        1.0f, 1.0f, 0.0f, ObjectInfo::bgR, ObjectInfo::bgG, ObjectInfo::bgB,
        1.0f, -1.0f, 0.0f, ObjectInfo::bgR, ObjectInfo::bgG, ObjectInfo::bgB
    };

    std::vector<unsigned int> indices2 = {
        0, 1, 2,
        1, 2, 3
    };

    vertexBuffer2 = std::make_unique<VertexBuffer>(vertices2.data(), vertices2.size() * sizeof(GLfloat));
    elementBuffer2 = std::make_unique<ElementBuffer>(indices2.data(), indices2.size() * sizeof(unsigned int));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

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

    imGuiGamePanel.reset(ImGuiGamePanel::getInstance());
    imGuiGamePanel->initImGui(gWindow.get()->getWindow(), glContext);
}

void Game::input() {
    static Keys* keys = Keys::getInstance();

    while (SDL_PollEvent(&gEvent)) {
        ImGui_ImplSDL2_ProcessEvent(&gEvent);
        gWindow.get()->handleEvent(gEvent);
        keys->handleInput(gEvent);
    }
}

void Game::update() {
    glViewport(0, 0, gWindow.get()->getWidth(), gWindow.get()->getHeight());

    gShader1.get()->bind();
    glBindVertexArray(VAO1);

    Uint32 degrees = (SDL_GetTicks() + 360) % 360;
    float radians = degrees * (M_PI / 180.0f);
    float newValue = cos(radians);
    gShader1->set3F("dynamicColor", newValue, newValue - 0.07f, newValue - 0.03f);

    gShader1->set3F("positionOffset", ObjectInfo::xPos, 0.0f, 0.0f);

    gShader2.get()->bind();
    glBindVertexArray(VAO2);

    gShader2->set4F("dynamicColor", ObjectInfo::bgR, ObjectInfo::bgG, ObjectInfo::bgB, ObjectInfo::bgA);

    spdlog::info("RGBA: {}, {}, {}, {}", ObjectInfo::bgR, ObjectInfo::bgG, ObjectInfo::bgB, ObjectInfo::bgA);
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    glBindVertexArray(VAO1);
    gShader1.get()->bind();

    glDrawElements(GL_TRIANGLES, elementBuffer1.get()->getCount(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAO2);
    gShader2.get()->bind();

    glDrawElements(GL_TRIANGLES, elementBuffer2.get()->getCount(), GL_UNSIGNED_INT, 0);

    imGuiGamePanel->render();
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