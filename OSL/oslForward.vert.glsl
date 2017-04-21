#version 430

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 positionOut;
out vec2 uvOut;
out vec3 normalOut;

uniform mat4 viewProjection;

void main(){
	gl_Position = viewProjection * vec4(position, 1);
	positionOut = position;
	uvOut = uv;
	normalOut = normal;
}