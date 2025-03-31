#pragma once
#include "SDL.h"
#include <glad/glad.h>
#include <memory>

class Shader;

class Game {

protected:
	Game();

public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

private:
	SDL_Event gEvent;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	std::unique_ptr<Shader> shader;
	int indicesCount;
public:
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_GLContext gOpenGLContext;
	bool running;

public:
	static Game* getInstance();

private:
	bool initSDLSubSystems();
	void setOpenGLAttributes();
	bool initGWindow();
	bool initOpenGL();
	bool initializeGladLibrary();
	void tutorial();

public:
	void printOpenGLVersionInfo();
	void initAll();
	void input();
	void update();
	void render() const;
	void clean() const;
};

