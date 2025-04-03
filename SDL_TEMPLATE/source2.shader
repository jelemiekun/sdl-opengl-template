#shader vertex
#version 430 core
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec3 aColor;

out vec4 objectColor;
void main() {
	gl_Position = vec4(aPos, 1.0f);
	objectColor = vec4(aColor, 1.0f);
}

#shader fragment
#version 430 core
out vec4 FragColor;
in vec4 objectColor;
uniform vec4 dynamicColor;
void main() {
	FragColor = vec4(dynamicColor);
}