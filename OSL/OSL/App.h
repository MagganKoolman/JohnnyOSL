#pragma once
#include "osl.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

class App {
struct vtxData{
	float a, b, c, x, y, z, u, v;
};
struct face {
	vtxData a, b, c;
};
private:
	int sphereSize;
	GLuint sphereVa;
	osl oslstuff;
	GLFWwindow* w;
public:
	App();
	~App();
	void run();
	void createSphereVBO(int resolution);
};