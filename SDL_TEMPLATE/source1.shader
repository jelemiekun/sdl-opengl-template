#shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 objectColor;
uniform vec3 positionOffset;

void main() {
	gl_Position = vec4(aPos + positionOffset, 1.0f);
	objectColor = vec3(aColor);
}

#shader fragment
#version 430 core
out vec4 FragColor;
in vec3 objectColor;
uniform vec3 dynamicColor;
void main() {
	FragColor = vec4(mix(objectColor, dynamicColor, 0.5f), 1.0f);
}