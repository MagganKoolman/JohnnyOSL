#version 430
 
in vec3 normalOut;
in vec2 uvOut;
out vec4 fragment_color;


void main(){
	fragment_color = vec4(uvOut.x, normalOut.x, 0.5, 1);
}