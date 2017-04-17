#version 430
 
flat in vec3 positionOut;
flat in vec3 normalOut;
in vec2 uvOut;
out vec4 fragment_color;

uniform image2D positionTex;
uniform image2D normalTex;

void main(){
	fragment_color = vec4(uvOut.xy, 0, 1);
	imageStore(positionTex, ivec2(uvOut), vec4(positionOut,1));
	imageStore(normalTex, ivec2(uvOut), vec4(normalOut,1));
}