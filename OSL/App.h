#pragma once
#include "osl.h"
#include "Forward.h"
#include "Camera.h"
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
	GLuint cubeVa;
	osl oslstuff;
	Forward forwardProgram;
	GLFWwindow* w;
	Camera camera;
public:
	App();
	~App();
	void run();
	void createSphereVBO(int resolution);
	void createCubeVBO();
};