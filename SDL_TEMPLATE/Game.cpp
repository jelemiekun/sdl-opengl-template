#include "Game.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <iostream>

Game::Game() : gWindow(nullptr), gRenderer(nullptr), gOpenGLContext(nullptr) {
	
}

Game* Game::getInstance() {
	static Game instance;
	return &instance;
}

void Game::setOpenGLAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

bool Game::initSDLSubSystems() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		spdlog::error("SDL could not initiailized: {}", SDL_GetError());
		return false;
	} else {
		spdlog::info("SDL initialized.");
		return true;
	}
}

bool Game::initGWindow() {
	gWindow = SDL_CreateWindow("RECAP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

	if (gWindow) {
		spdlog::info("SDL Window created successfully.");
		return true;
	} else {
		spdlog::error("Failed to create SDL Window: {}", SDL_GetError());
		return false;
	}
}

bool Game::initOpenGL() {
	gOpenGLContext = SDL_GL_CreateContext(gWindow);

	if (gOpenGLContext) {
		spdlog::info("Open GL Context created successfully.");
		return true;
	} else {
		spdlog::error("Failed to create Open GL Context: {}", SDL_GetError());
		return false;
	}
}

bool Game::initializeGladLibrary() {
	if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		spdlog::info("Glad Library successfully initialized.");
		return true;
	} else {
		spdlog::info("Failed to initialize Glad Library.");
		return false;
	}
}

void Game::printOpenGLVersionInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "Shading Languag: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}


void Game::initAll() {
	if (!initSDLSubSystems()) return;

	setOpenGLAttributes();

	if (!initGWindow()) return;

	if (!initOpenGL()) return;

	if (!initializeGladLibrary()) return;

	running = true;
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			running = false;
		}
	}
}

void Game::update() {

}

void Game::render() {
	SDL_GL_SwapWindow(gWindow);
}

void Game::clean() const {
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}