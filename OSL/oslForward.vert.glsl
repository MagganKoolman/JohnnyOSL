#version 430

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 positionOut;
out vec2 uvOut;

void main(){
	gl_Position = vec4(position.xz,0, 1);
	positionOut = position;
	vec3 a = normal;
	uvOut = uv;
}