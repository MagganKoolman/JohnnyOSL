#version 430

in vec3 position;
in vec3 normal;
in vec2 UVs;

out vec3 posOut;
out vec3 normalOut;
out vec2 UVout;

layout(location = 0) uniform mat4 viewProjection;
layout(location = 4) uniform mat4 world;

void main(){
	posOut = vec3(world * vec4(position, 1));
	normalOut = normal;
	gl_Position = viewProjection * world * vec4(position, 1);
	UVout = UVs;
}