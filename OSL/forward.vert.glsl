#version 430

in vec3 position;
in vec3 normal;

out vec3 positionOut;
out vec3 normalOut;

uniform mat4 viewProjection;
uniform mat4 world;

void main(){
	gl_Position = viewProjection * world * vec4(position, 1);
	normalOut = normal;
}