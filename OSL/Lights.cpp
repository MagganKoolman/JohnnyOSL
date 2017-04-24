#include "Lights.h"
#include <iostream>
Lights::Lights()
{

}

Lights::~Lights()
{
	delete[] allLights;
}

void Lights::update(GLuint programID, float dt)
{
	timer += dt;
	allLights[0].position.x = 10 + 10 * cosf(timer / 5);
	allLights[0].position.z = 10 + 10 * sinf(timer / 6);
	allLights[1].position.x = 25 + 15 * cosf(timer / 1);
	allLights[1].position.z = 25 + 15 * sinf(timer / 1.3);

	glBindBuffer(GL_UNIFORM_BUFFER, this->lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(aLight) * 2, this->allLights, GL_DYNAMIC_DRAW);

	GLint lightLoc = glGetUniformBlockIndex(programID, "Lights");
	glBindBufferBase(GL_UNIFORM_BUFFER, lightLoc, this->lightBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Lights::init(GLuint programID, int nrOfLights)
{
	this->allLights = new aLight[nrOfLights];
	GLint lightLoc = glGetUniformBlockIndex(programID, "Lights");
	GLint blockSize;
	glGetActiveUniformBlockiv(programID, lightLoc, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	
	if (blockSize != sizeof(aLight) * nrOfLights)
		std::cout << "Light uniform block and CPU buffer are not same size!!!! Bad!!!\n";

	glGenBuffers(1, &this->lightBuffer);
	
	for (int i = 0; i < nrOfLights; i++)
	{
		this->allLights[i].position = vec4(20 * i, 0.5, 20 * i, 5 * (i+1));
		this->allLights[i].colora = vec4(0.5, 0.5, 0.5, 1);
	}
}
