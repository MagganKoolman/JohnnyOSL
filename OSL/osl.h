#pragma once
#include <GL\glew.h>
#include <GL\GL.h>

class osl {

//"private" variables
struct oslObject{
	GLuint positionTex, normalTex, diffuseTex;
};
public:
	const int textureRes = 32;
	GLuint oslprog, textureGenProg;
	oslObject sphere, cube;

public:
	osl();
	~osl();
	void init();
	void render(GLuint va, int size);
	void generateTextures(GLuint va, int size);
};