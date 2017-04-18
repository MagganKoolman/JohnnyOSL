#version 430

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 positionOut;
out vec3 normalOut;
out vec2 uvOut;

uniform mat4 viewProjection;

void main(){
	gl_Position = viewProjection * mat4(1) * vec4(position, 1);
	normalOut = normal;
	uvOut = uv;
}