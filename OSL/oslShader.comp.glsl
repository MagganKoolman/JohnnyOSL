#version 430

layout(binding = 0, rgba32f) uniform image2D positionTex;
layout(binding = 1, rgba32f) uniform image2D normalTex;
layout(binding = 2, rgba32f) uniform image2D srcTex;
layout(binding = 3, rgba32f) uniform image2D destTex;


layout(local_size_x = 4, local_size_y = 4) in;

uniform vec3 camPos;
uniform mat4 world;

struct Light{
	vec4 position;
	vec4 colora;
};

const int nrOfLights = 20;

layout(std140) uniform Lights
{
  Light lights[nrOfLights];
};


void main(){
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	vec3 pos = (world * vec4(imageLoad(positionTex, storePos).xyz,1)).xyz;
	vec3 normal = imageLoad(normalTex, storePos).xyz;
	vec4 color = imageLoad(srcTex, storePos);

	float diffuse = 0;
	float specular = 0;
	float attunuation;
	for( int i =0; i < nrOfLights; i++){
		if(length(lights[i].position.xyz - pos) < lights[i].position.w){  
			vec3 diffuseVec = normalize(lights[i].position.xyz - pos);
			diffuse += dot(diffuseVec, normal);

			vec3 eyeDir = normalize(lights[i].position.xyz - camPos);
			vec3 vHalfVector = reflect(diffuseVec, normal);
			specular += pow(max(dot(eyeDir, vHalfVector),0.0), 20);
		}
	}

	color = vec4((diffuse+specular)*color.xyz, 1); // diffuse+specular
	imageStore(destTex, storePos, vec4(color.xyz, 1));
}