#pragma once
#include <GL\glew.h>
#include <GL\GL.h>

class Forward {
public:
	Forward();
	~Forward();
	void init();
	void render(GLuint va, int size);
private:
	GLuint programID;
};
