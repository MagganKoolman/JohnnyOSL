#version 430
 
in vec3 normalOut;
in vec3 posOut;
in vec2 UVout;

out vec4 fragment_color;

layout (location = 8) uniform vec3 cameraPos;

uniform sampler2D someTex;

struct Light{
	vec4 position;
	vec4 colora;
};

const int nrOfLights = 2;

layout(std140) uniform Lights
{
  Light lights[nrOfLights];
};

void main(){
/*
	vec3 aLight = {10, -0.5, 10};
	vec3 diffuseVec = normalize(aLight - posOut);
	float diffuse = dot(diffuseVec, normalOut);

	vec3 eyeDir = normalize(posOut - cameraPos);
	vec3 vHalfVector = reflect(diffuseVec, normalOut);
    float specular = pow(max(dot(eyeDir, vHalfVector),0.0), 20);
*/
	vec4 color = texture(someTex, UVout);
	fragment_color = color;
	/*vec3 diffuseVec;
	float diffuse;
	vec3 eyeDir;
	vec3 vHalfVector;
	float specular;
	for (int i = 0; i < nrOfLights; i++)
	{
		if (length(lights[i].position.xyz - posOut) < lights[i].position.w)
		{
			diffuseVec = normalize(lights[i].position.xyz - posOut);
			diffuse = dot(diffuseVec, normalOut);

			eyeDir = normalize(lights[i].position.xyz  - cameraPos);
			vHalfVector = reflect(diffuseVec, normalOut);
			specular = pow(max(dot(eyeDir, vHalfVector),0.0), 20);
			fragment_color += color * diffuse + color * specular + lights[i].colora;
		}
	}*/
	//fragment_color = vec4(UVout,0,1);
}