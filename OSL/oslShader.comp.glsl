#version 430

layout(binding = 0, rgba32f) uniform image2D positionTex;
layout(binding = 1, rgba32f) uniform image2D normalTex;
layout(binding = 2, rgba32f) uniform image2D srcTex;
layout(binding = 3, rgba32f) uniform image2D destTex;


layout(local_size_x = 8, local_size_y = 8) in;

layout(location = 1) uniform vec3 camPos;
layout(location = 2) uniform mat4 world;

struct Light{
	vec4 position;
	vec4 colora;
};

const int nrOfLights = 10;

layout(std140) uniform Lights
{
	Light lights[nrOfLights];
};

layout(location = 7) uniform int indices[10];

layout(location = 6) uniform int activeLights;

layout(location = 5) uniform int megaTexIndex;

void main(){
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	vec3 pos = (world * vec4(imageLoad(positionTex, storePos).xyz,1)).xyz;
	vec3 normal = imageLoad(normalTex, storePos).xyz;
	vec4 color = imageLoad(srcTex, storePos);

	int xOffset = (megaTexIndex%32);
	int yOffset = megaTexIndex / 32;
	storePos += ivec2(xOffset*128, yOffset*128);

	float diffuse = 0;
	float specular = 0;

	vec3 eyeDir;
	vec3 vHalfVector;
	vec3 lightpos;
	vec3 diffuseVec;
	for( int i = 0; i < nrOfLights; i++){
		lightpos = lights[i].position.xyz;
		if(length(lightpos - pos) < lights[i].position.w){  
			diffuseVec = normalize(lightpos - pos);
			diffuse += dot(diffuseVec, normal);

			eyeDir = normalize(lightpos - camPos);
			vHalfVector = reflect(diffuseVec, normal);
			specular += pow(max(dot(eyeDir, vHalfVector),0.0), 20);
		}
	}
	diffuse = max(diffuse, 0.0);
	color = (0.2+diffuse+specular)*color; 
	imageStore(destTex, storePos, color);
}