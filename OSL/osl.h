#pragma once
#include <GL\glew.h>
#include <GL\GL.h>

class osl {

//"private" variables
struct oslObject{
	GLuint positionTex, normalTex, diffuseTex;
};
public:
	const int textureRes = 64;
	GLuint oslprog, textureGenProg, oslForward;
	oslObject sphere, cube;

public:
	osl();
	~osl();
	void init();
	void render(GLuint va, int size);
	void generateTextures(GLuint va, int size);
};