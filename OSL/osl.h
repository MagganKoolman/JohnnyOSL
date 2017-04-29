#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <glm\common.hpp>
#include "Lights.h"

class osl {

	struct oslObject{
		GLuint positionTex, normalTex, diffuseTex;
		GLuint va;
		int size;
	};
	struct oslInstance {
		GLuint tex;
	};

	
	public:			//"private" variables
		const int textureRes = 256;
		static const int maxObjects = 5000;
		GLuint oslprog, textureGenProg, oslForward;
		oslObject sphere, cube;
		int nrOfSpheres;
		int nrOfCubes;
		oslInstance sphereInstances[maxObjects];
		oslInstance cubeInstances[maxObjects];
		glm::mat4* cubes;
		glm::mat4* spheres;
		Lights lights;

		GLuint lockTex;
	public:
		osl();
		~osl();
		void init();
		void render( glm::mat4 vp, glm::vec3 camPos);
		void renderInstances(glm::mat4 vp);
		void updateShading(glm::vec3 camPos, oslObject object, oslInstance instance, glm::mat4 world);
		void generateTextures(GLuint sphereVa, int sphereSize);
		void createSphereTextures(int number);
		void createCubeTextures(int number);
};