#version 430
 
in vec2 uvOut;
in vec3 normalOut;

out vec4 fragment_color;

layout(location = 0) uniform sampler2D diffTex;



void main(){
	vec2 uv = uvOut.xy + normalOut.xy-normalOut.xy;
	fragment_color = vec4(texture(diffTex, uv).xyz, 1);
}