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
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}

void Game::tutorial() {
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec2 firstTwoColor;\n"
		"out vec4 triColor;\n"
		"out vec3 vertexColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"	firstTwoColor = vec2(0.0f, 0.9f);\n"
		"	triColor = vec4(firstTwoColor.x, firstTwoColor.y, 1.0f, 0.0f);\n"
		"	vertexColor = aColor;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"in vec4 triColor;\n"
		"in vec3 vertexColor;\n"
		"out vec4 FragColor;\n"
		"uniform vec4 timeColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(vertexColor, timeColor.w);\n"
		"}\n\0";

	glViewport(0, 0, 640, 480);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint vertexSuccess;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
	if (vertexSuccess) {
		spdlog::info("Vertex shader successfully compiled.");
	} else {
		char log[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		spdlog::warn("Vertex shader failed to compile: {}", log);
	}

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint fragmentSuccess;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
	if (fragmentSuccess) {
		spdlog::info("Fragment shader successfully compiled.");
	} else {
		char log[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		spdlog::warn("Fragment shader failed to compile: {}", log);
	}

	// Shader programm
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint linkingSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingSuccess);
	if (linkingSuccess) {
		spdlog::info("Shader program linked successfully.");
	} else {
		char log[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		spdlog::warn("Shader program failed to link: {}", log);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertices
	std::vector<GLfloat> vertices = {
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.0f, 0.0f, 0.4f, 0.2f, 0.9f
	};

	std::vector<GLuint> indices = {
		0, 1, 2, 
		0, 1, 3
	};

	indicesCount = indices.size();

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);


	// EBO
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

void Game::render() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(VAO); // Use the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Use the EBO
	glUseProgram(shaderProgram); // Use the shader program

	Uint32 timeValue = SDL_GetTicks();
	float blueValue = (sin(timeValue / 2.0f) + 0.5f);
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "timeColor");
	glUniform4f(vertexColorLocation, 0.0f, 0.0f, blueValue, 1.0f);

	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);  // Draw shape
	
	SDL_GL_SwapWindow(gWindow);

	SDL_Delay(500);
}

void Game::clean() const {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}