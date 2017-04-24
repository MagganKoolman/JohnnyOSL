#version 430

uniform vec3 lightPos;
uniform vec3 camPos;
layout(binding = 0, rgba32f) uniform image2D positionTex;
layout(binding = 1, rgba32f) uniform image2D normalTex;
layout(binding = 2, rgba32f) uniform image2D srcTex;
layout(binding = 3, rgba32f) uniform image2D destTex;


layout(local_size_x = 4, local_size_y = 4) in;

void main(){
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	vec3 pos = imageLoad(positionTex, storePos).xyz;
	vec3 normal = normalize(imageLoad(normalTex, storePos).xyz);
	vec4 color = imageLoad(srcTex, storePos);
	vec3 lightvec = normalize(lightPos - pos);
	vec3 diffuse = abs(dot(normal, lightvec))* color.rgb;
	vec3 specular = abs(dot(normalize(camPos - pos), normalize(reflect(lightvec, normal)))) * color.rgb;
	color = vec4(normal, 1); // diffuse+specular
	imageStore(destTex, storePos, color);
}