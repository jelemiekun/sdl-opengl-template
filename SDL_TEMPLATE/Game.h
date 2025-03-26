#pragma once
#include "SDL.h"

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

public:
	void printOpenGLVersionInfo();

	void initAll();
	void input();
	void update();
	void render();
	void clean() const;
};

