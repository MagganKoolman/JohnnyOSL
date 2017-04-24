#version 430
 
in vec3 positionOut;
in vec3 normalOut;
in vec2 uvOut;
out vec4 fragment_color;

layout(binding = 0, rgba32f) uniform image2D positionTex;
layout(binding = 1, rgba32f) uniform image2D normalTex;

void main(){
	imageStore(positionTex, ivec2(uvOut*imageSize(positionTex)), vec4(positionOut, 1));
	imageStore(normalTex, ivec2(uvOut*imageSize(normalTex)), vec4(normalOut, 1));
	fragment_color = vec4(positionOut.xy, 0, 0);
}