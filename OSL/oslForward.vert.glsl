#version 430

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec2 uvOut;
out vec3 normalOut;

uniform mat4 viewProjection;
uniform mat4 world;


void main(){
	gl_Position = viewProjection * world * vec4(position, 1);
	uvOut = uv;

	normalOut = normal;

}