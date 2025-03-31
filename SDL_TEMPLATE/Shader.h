#pragma once
#include <glad/glad.h>
#include <iostream>

class Shader {
private:
	enum class SHADER_TYPE {
		None,
		Vertex,
		Fragment
	};

	std::string parseShaderSource(const char* sourcePath, SHADER_TYPE type);
	GLuint compileShader(GLuint shader_type, const char* source);
	bool usable;

public:
	unsigned int ID;

	Shader(const char* sourcePath);

	void use() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void clean();
};

