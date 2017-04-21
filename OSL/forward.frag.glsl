#version 430
 
in vec3 normalOut;
in vec3 posOut;
in vec2 UVout;

out vec4 fragment_color;

uniform vec3 cameraPos;
uniform sampler2D someTex;

void main(){
	vec3 aLight = {10, -0.5, 10};
	vec3 diffuseVec = normalize(aLight - posOut);
	float diffuse = dot(diffuseVec, normalOut);

	vec3 eyeDir = normalize(posOut - cameraPos);
	vec3 vHalfVector = reflect(diffuseVec, normalOut);
    float specular = pow(max(dot(eyeDir, vHalfVector),0.0), 20);

	vec4 color = texture(someTex, UVout);
	fragment_color = color * diffuse + color * specular;
}