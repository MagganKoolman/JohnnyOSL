#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <glm\glm.hpp>

class Forward {
public:
	Forward();
	~Forward();
	void init();
	void render(GLuint va, glm::mat4 viewProj);
	glm::mat4* cubes;
	glm::mat4* spheres;
	GLuint cubeVao;
	GLuint sphereVao;
private:
	GLuint programID;
};
