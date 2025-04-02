#include "Game.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <iostream>
#include "Shader.h"
#include "ImGuiWindow.h"
#include "imgui/imgui_impl_sdl2.h"

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
	gWindow = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

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
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}

void Game::tutorial() {
	shader = std::make_unique<Shader>("source.shader");

	glViewport(0, 0, 640, 480);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<GLfloat> vertices = {
		// h
		-0.8f, 0.8f, 0.0f, 0.98f, 0.706f, 0.933f,
		-0.75f, 0.8f, 0.0f, 0.831f, 0.831f, 0.831f,
		-0.8f, 0.4f, 0.0f, 1.0f, 0.373f, 0.894f,
		-0.75f, 0.4f, 0.0f,0.98f, 0.71f, 0.76f,

		-0.75f, 0.65f, 0.0f,1.00f, 0.41f, 0.71f,
		-0.6f, 0.65f, 0.0f,1.00f, 0.08f, 0.58f,
		-0.75f, 0.6f, 0.0f,1.00f, 0.62f, 0.67f,
		-0.6f, 0.6f, 0.0f,1.00f, 0.76f, 0.80f,

		-0.6f, 0.8f, 0.0f,1.00f, 0.00f, 0.50f,
		-0.55f, 0.8f, 0.0f,1.00f, 0.57f, 0.64f,
		-0.6f, 0.4f, 0.0f,1.00f, 0.00f, 1.00f,
		-0.55f, 0.4f, 0.0f,1.00f, 0.00f, 0.64f,

		// i
		-0.45f, 0.75f, 0.0f,0.97f, 0.51f, 0.47f,
		-0.4f, 0.75f, 0.0f,0.88f, 0.69f, 1.00f,
		-0.45f, 0.65f, 0.0f,0.85f, 0.44f, 0.89f,
		-0.4f, 0.65f, 0.0f,1.00f, 0.82f, 0.86f,

		-0.45f, 0.6f, 0.0f,1.00f, 0.65f, 0.79f,
		-0.4f, 0.6f, 0.0f,1.00f, 0.00f, 0.64f,
		-0.45f, 0.4f, 0.0f,1.00f, 0.08f, 0.58f,
		-0.4f, 0.4f, 0.0f,1.00f, 0.72f, 0.77f
	};

	std::vector<GLuint> indices = {
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,

		8, 9, 10,
		9, 10, 11,

		12, 13, 14,
		13, 14, 15,

		16, 17, 18,
		17, 18, 19,
	};

	indicesCount = indices.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


void Game::initAll() {
	if (!initSDLSubSystems()) return;

	setOpenGLAttributes();

	if (!initGWindow()) return;

	if (!initOpenGL()) return;

	if (!initializeGladLibrary()) return;

	tutorial();

	ImGuiWindowContext = ImGuiWindow::getInstance();
	ImGuiWindowContext->init(gWindow, &gOpenGLContext);

	running = true;
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		ImGui_ImplSDL2_ProcessEvent(&gEvent);

		if (gEvent.type == SDL_QUIT) {
			running = false;
		}
	}
}

void Game::update() {
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	shader->use();
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	ImGuiWindowContext->render();
	SDL_GL_SwapWindow(gWindow);

	SDL_Delay(1);
}

void Game::clean() const {
	shader->clean();
	ImGuiWindowContext->clean();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}