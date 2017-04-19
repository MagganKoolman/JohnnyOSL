#version 430
 
in vec3 normalOut;
out vec4 fragment_color;

void main(){
	fragment_color = vec4(normalOut, 1);
}