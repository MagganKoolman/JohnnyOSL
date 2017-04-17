#version 430

uniform vec3 lightPos;
uniform vec3 camPos;
layout(binding = 0, rgb32f) uniform image2D positionTex;
layout(binding = 1, rgb32f) uniform image2D normalTex;
layout(binding = 2, rgba32f) uniform image2D destTex;

layout(local_size_x = 1, local_size_y = 1) in;

void main(){
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	vec3 pos = imageLoad(positionTex, storePos).xyz;
	vec3 normal = imageLoad(positionTex, storePos).xyz;
	vec4 color = imageLoad(destTex, storePos);
	vec3 lightvec = lightPos - pos;
	color = vec4(dot(normal, lightvec)* color.xyz + pow(dot(camPos - pos, reflect(lightvec, normal)),10), color.w);
	imageStore(destTex, storePos, color);
}