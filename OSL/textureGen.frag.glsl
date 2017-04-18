#version 430
 
flat in vec3 positionOut;
flat in vec3 normalOut;
in vec2 uvOut;
out vec4 fragment_color;

layout(binding = 0, rgba32f) uniform image2D positionTex;
layout(binding = 1, rgba32f) uniform image2D normalTex;

void main(){
	imageStore(positionTex, ivec2(uvOut*64), vec4((positionOut+vec3(1,1,1))/2, 1));
	imageStore(normalTex, ivec2(uvOut*64), vec4((normalOut+vec3(1,1,1))/2, 1));
	fragment_color = vec4(positionOut.xy, 0, 1);
}