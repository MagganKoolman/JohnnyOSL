#version 430

in vec3 position;
in vec3 normal;
in vec2 uv;

flat out vec3 positionOut;
flat out vec3 normalOut;
out vec2 uvOut;

void main(){
	gl_Position = vec4(position*2, 1);
	positionOut = position;
	normalOut = normal;
	uvOut = uv;
}