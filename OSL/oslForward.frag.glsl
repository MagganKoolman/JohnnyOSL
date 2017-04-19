#version 430
 
in vec3 positionOut;
in vec2 uvOut;

out vec4 fragment_color;

uniform sampler2D diffTex;

void main(){
	fragment_color = vec4(texture(diffTex, uvOut).xyz, 1);
}