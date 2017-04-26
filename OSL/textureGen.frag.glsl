#version 450
 
in vec3 positionOut;
in vec3 normalOut;
in vec2 uvOut;

out vec4 fragment_color;

layout(binding = 0, rgba32f) uniform image2D positionTex;
layout(binding = 1, rgba32f) uniform image2D normalTex;
layout(binding = 2, r32ui) uniform uimage2D lockTex;


void takeLock(ivec2 pos){
	uint lockAvailable;
	lockAvailable = imageAtomicCompSwap(lockTex, pos, uint(0), uint(1));
	if (lockAvailable == 1){
		discard;
	}
}
void releaseLock(ivec2 pos){
	imageStore(lockTex, pos, uvec4(0));
}

void main(){
	ivec2 size = imageSize(positionTex);
	ivec2 imageCoord = ivec2(uvOut * size);

	//takeLock(imageCoord);

	imageStore(positionTex, imageCoord, vec4(positionOut, 0));
	imageStore(normalTex, imageCoord, vec4(normal, 0));
	
	//releaseLock(imageCoord);

	fragment_color = vec4(normalOut, 0);
}