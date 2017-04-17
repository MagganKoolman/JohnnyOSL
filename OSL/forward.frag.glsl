#version 430
 
in vec3 positionOut;
in vec3 normalOut;
in vec2 uvOut;
out vec4 fragment_color;


void main(){
	fragment_color = vec4(uvOut.xy, 0.5, 1);
}