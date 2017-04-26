#version 450

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec2 uvOut;
out vec3 normalOut;

uniform mat4 viewProjection;

void main(){
	gl_Position = viewProjection * vec4(position, 1);
	uvOut = uv;
	normalOut = normal;

}