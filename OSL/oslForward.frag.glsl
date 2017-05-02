#version 430
 
in vec2 uvOut;
in vec3 normalOut;

out vec4 fragment_color;

uniform sampler2D diffTex;

uniform int megaTexIndex;


void main(){
	vec2 uv = uvOut.xy + normalOut.xy-normalOut.xy;
	uv=uv/32;
	uv.x += (1/32) * mod(megaTexIndex, 32);
	uv.y += (1/32) * megaTexIndex/32;
	fragment_color = vec4(texture(diffTex, uv).xyz, 1);
	//fragment_color = vec4(uvOut, 0, 1);
}