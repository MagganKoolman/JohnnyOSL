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
	const int textureRes = 64;
	GLuint oslprog, textureGenProg, oslForward;
	oslObject sphere, cube;
	GLuint sphereTex; //temporary
public:
	osl();
	~osl();
	void init();
	void render(GLuint va, int size, glm::mat4 vp);
	void generateTextures(GLuint va, int size);
};