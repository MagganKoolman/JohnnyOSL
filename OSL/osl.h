#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <glm\common.hpp>
#include "Lights.h"
#include <string>
#include "Hitbox.h"

class osl {
	struct oslObject{
		GLuint positionTex, normalTex, diffuseTex;
		GLuint va;
		int size;
	};
	struct oslInstance {
		GLuint tex;
		Hitbox hb;
		int32_t lights[10];
		int lightsAffecting;
		bool fixed;
	};

	
	public:			//"private" variables
		const int textureRes = 128;
		static const int maxObjects = 500;
		GLuint oslprog, textureGenProg, oslForward;
		oslObject sphere, cube;
		int nrOfSpheres;
		int nrOfCubes;
		glm::ivec2 sphereTexSize;
		oslInstance sphereInstances[maxObjects];
		oslInstance cubeInstances[maxObjects];
		glm::mat4* cubes;
		glm::mat4* spheres;
		Lights lights;
		const int nrOfLights = 10;
		GLuint indexBuffer;

		GLuint megaTex;

		GLuint lockTex;
	public:
		osl();
		~osl();
		void init();
		void render( glm::mat4 vp, glm::vec3 camPos, float dt);
		void renderInstances(glm::mat4 vp);
		void updateShading(oslObject &object, oslInstance &instance, glm::mat4 &world, int index);
		void generateTextures(GLuint sphereVa, int sphereSize, oslObject object);
		void createSphereTextures(int number);
		void createCubeTextures(int number);
		void createTextures(oslObject* obj, std::string path);
		void updateLights(oslInstance* instance, int number);
		void setupShading(glm::vec3 &camPos, oslObject* obj);
};