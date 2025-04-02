#pragma once
#include <SDL.h>
#include <memory>

class Window;
class Shader;

class Game {
private:
    Game();

public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    static Game* getInstance();

private:
    bool running;
    std::unique_ptr<Window> gWindow;
    SDL_GLContext glContext;
    std::unique_ptr<Shader> gShader;
    SDL_Event gEvent;

private:
    bool initSDL();
    bool initSDLWindow();
    void setOpenGLAttributes();
    bool initOpenGL();
    bool initGlad();
    void setupShader();

public:
    void init();

    void input();
    void update();
    void render();

    void quit();
    const bool& isRunning() const;

    void clean();
};

