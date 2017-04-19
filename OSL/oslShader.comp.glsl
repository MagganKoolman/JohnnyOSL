#version 430

uniform vec3 lightPos;
uniform vec3 camPos;
layout(binding = 0, rgba32f) uniform image2D positionTex;
layout(binding = 1, rgba32f) uniform image2D normalTex;
layout(binding = 2, rgba32f) uniform image2D destTex;

layout(local_size_x = 4, local_size_y = 4) in;

void main(){
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	vec3 pos = imageLoad(positionTex, storePos).xyz;
	vec3 normal = imageLoad(normalTex, storePos).xyz;
	vec4 color = imageLoad(destTex, storePos);
	vec3 lightvec = lightPos - pos;
	vec3 diffuse = abs(dot(normal, lightvec))* vec3(1,0,0);
	vec3 specular = abs(dot(camPos - pos, reflect(lightvec, normal))) * vec3(1,0,0);
	color = vec4(diffuse+specular, 1);
	imageStore(destTex, storePos, color);
}