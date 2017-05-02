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

const int nrOfLights = 10;

layout(std140) uniform Lights
{
	Light lights[nrOfLights];
};

uniform int indices[10];

uniform int activeLights;

void main(){
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	vec3 pos = (world * vec4(imageLoad(positionTex, storePos).xyz,1)).xyz;
	vec3 normal = imageLoad(normalTex, storePos).xyz;
	vec4 color = imageLoad(srcTex, storePos);

	float diffuse = 0;
	float specular = 0;
	float attunuation;
	int index = 0;
	vec3 lightpos;
	for( int i = 0; i < activeLights; i++){
		index = indices[i];
		lightpos = lights[index].position.xyz;
		if(length(lightpos - pos) < lights[index].position.w){  
			vec3 diffuseVec = normalize(lightpos - pos);
			diffuse += dot(diffuseVec, normal);

			vec3 eyeDir = normalize(lightpos - camPos);
			vec3 vHalfVector = reflect(diffuseVec, normal);
			specular += pow(max(dot(eyeDir, vHalfVector),0.0), 20);
		}
	}
	diffuse = max(diffuse, 0.0);
	specular = max(specular, 0.0);
	color = (0.2+diffuse+specular)*color; // diffuse+specular
	//color = vec4(activeLights);
	imageStore(destTex, storePos, color);
}