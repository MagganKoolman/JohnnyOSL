#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <glm\common.hpp>

class osl {

//"private" variables
struct oslObject{
	GLuint positionTex, normalTex, diffuseTex;
};
public:
	const int textureRes = 512;
	GLuint oslprog, textureGenProg, oslForward;
	oslObject sphere, cube;
	GLuint sphereTex; //temporary
	GLuint lockTex;
public:
	osl();
	~osl();
	void init();
	void render(GLuint va, int size, glm::mat4 vp, glm::vec3 camPos);
	void generateTextures(GLuint va, int size);
};