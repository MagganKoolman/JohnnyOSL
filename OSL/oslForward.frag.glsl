#version 430
 
in vec2 uvOut;
in vec3 normalOut;

out vec4 fragment_color;

uniform sampler2D diffTex;

void main(){
	//fragment_color = vec4(texture(diffTex, uvOut).xyz, 1);
	fragment_color = vec4(normalOut, 1);
}