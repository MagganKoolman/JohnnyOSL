#version 430
 
in vec3 positionOut;
in vec3 normalOut;
in vec2 uvOut;
out vec4 fragment_color;


void main(){
	fragment_color = vec4(uvOut.x, 0.4, 0.5, 1);
}