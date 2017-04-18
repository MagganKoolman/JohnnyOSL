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
private:
	GLuint programID;
};
