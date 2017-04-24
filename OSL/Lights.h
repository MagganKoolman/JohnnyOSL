#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"

#include "GLFW\glfw3.h"

using namespace glm;
struct aLight {
	vec4 position, colora;
};
class Lights {
public:
	Lights();
	~Lights();

	void update(GLuint programID, float dt);
	void init(GLuint programID, int nrOfLights);

private:
	aLight* allLights;
	GLuint lightBuffer;
	float timer;
};