#version 450
 
in vec2 uvOut;
in vec3 normalOut;

out vec4 fragment_color;

uniform sampler2D diffTex;

void main(){
	vec2 uv = uvOut.xy + normalOut.xy-normalOut.xy;
	fragment_color = vec4(texture(diffTex, uv).xyz, 1);
	//fragment_color = vec4(uvOut, 0, 1);
}