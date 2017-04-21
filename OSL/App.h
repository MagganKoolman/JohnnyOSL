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
struct betterData {
	glm::vec3 pos, normals;
};
struct betterFace {
	betterData f1, f2, f3;
};

private:
	int sphereSize;
	GLuint sphereVa;
	GLuint cubeVa;
	osl oslstuff;
	Forward forwardProgram;
	GLFWwindow* w;
	Camera camera;
	glm::mat4 cubeMatrices[25*25];
	glm::mat4 sphereMatrices[25*25];


	glm::vec3 movement = { 0,0,0 };
public:
	App();
	~App();
	void run();
	GLuint createSphereVBO(int resolution);
	GLuint createCubeVBO();
	void createCubes();
	void createSpheres();
	void updateInputs();
};