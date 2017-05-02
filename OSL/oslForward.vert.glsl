#version 430

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec2 uvOut;
out vec3 normalOut;

uniform mat4 viewProjection;
uniform mat4 world;

uniform int megaTexIndex;

void main(){
	gl_Position = viewProjection * world * vec4(position, 1);
	uvOut = uv/32;
	
	uvOut += vec2(
				(1.0/32.0) * (megaTexIndex % 32),		//x
				(1.0/32.0) * (megaTexIndex/32)			//y
	);

	normalOut = normal;

}