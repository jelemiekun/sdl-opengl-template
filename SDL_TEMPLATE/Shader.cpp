#include "Shader.h"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const char* sourcePath) : usable(false) {
	std::string vertexShaderSource = parseShaderSource(sourcePath, SHADER_TYPE::Vertex);
	std::string fragmentShaderSource = parseShaderSource(sourcePath, SHADER_TYPE::Fragment);

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

	if (vertexShader == 0 || fragmentShader == 0) {
		spdlog::error("Failed to create a usable shader.");
		return;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glValidateProgram(ID);

	int shaderProgramSuccess;
	glGetProgramiv(ID, GL_LINK_STATUS, &shaderProgramSuccess);
	if (shaderProgramSuccess) {
		spdlog::info("Shader program linked successfully.");
		usable = true;
	} else {
		char log[512];
		glGetProgramInfoLog(ID, 512, NULL, log);
		spdlog::warn("Shader program failed to link: {}", log);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::clean() {
	spdlog::info("Cleaning shader...");
	glDeleteProgram(ID);
}

std::string Shader::parseShaderSource(const char* sourcePath, SHADER_TYPE type) {
	std::ifstream stream(sourcePath);
	if (!stream) {
		spdlog::error("Failed to open shader file: {}", sourcePath);
		return "";
	}

	SHADER_TYPE currentType = SHADER_TYPE::None;
	std::stringstream vertexCode, fragmentCode;
	std::string line;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				currentType = SHADER_TYPE::Vertex;
			} else if (line.find("fragment") != std::string::npos) {
				currentType = SHADER_TYPE::Fragment;
			}
		} else {
			if (currentType == SHADER_TYPE::Vertex) {
				vertexCode << line << '\n';
			} else if (currentType == SHADER_TYPE::Fragment) {
				fragmentCode << line << '\n';
			}
		}
	}

	return (type == SHADER_TYPE::Vertex) ? vertexCode.str() : fragmentCode.str();
}


GLuint Shader::compileShader(GLuint shader_type, const char* source) {
	GLuint shader = glCreateShader(shader_type);

	if (shader == 0) {
		spdlog::error("Failed to create {} Shader.", (shader_type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment");
		return 0;
	}

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int shaderSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
	std::string shaderTypeString = (shader_type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
	if (shaderSuccess) {
		spdlog::info("{} Shader compiled successfully.", shaderTypeString);
		return shader;
	} else {
		char log[512];
		glGetShaderInfoLog(shader, 512, NULL, log);
		spdlog::warn("{} Shader failed to compile: {}", shaderTypeString, log);
		return 0;
	}
}

void Shader::use() const {
	if (usable)
		glUseProgram(ID);
	else
		spdlog::warn("Unusable shader program.");
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float  value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}