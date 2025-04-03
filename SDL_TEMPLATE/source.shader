#shader vertex
#version 410 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec2 aTexCoord;

	out vec3 vertexColor;
	out vec2 texCoord;

	void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		vertexColor = aColor;
		texCoord = vec2(aTexCoord);
	}

#shader fragment
#version 410 core
	out vec4 FragColor;
	in vec2 texCoord;
	in vec3 vertexColor;
	uniform sampler2D texture1;
	void main(){
		FragColor = texture(texture1, texCoord);
	}