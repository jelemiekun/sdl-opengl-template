#shader vertex
#version 410 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	out vec3 vertexColor;
	
	void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		vertexColor = aColor;
	}

#shader fragment
#version 410 core
	out vec4 FragColor;
	in vec3 vertexColor;
	void main(){
		FragColor = vec4(vertexColor, 1.0f);
	}