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
	for (int i = 0; i < nrOfLight; i++) {
		allLights[i].position.x = ((i*2)%50) + i * cosf(timer / ((i*3)%5));
		allLights[i].position.z = i + i * sinf(timer / ((i*2)%7));
	}

	glBindBuffer(GL_UNIFORM_BUFFER, this->lightBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(aLight) * nrOfLight, this->allLights, GL_DYNAMIC_DRAW);

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
		this->allLights[i].position = vec4(20 * i, 0.5, 20 * i, 5);
		this->allLights[i].colora = vec4(0.5, 0.5, 0.5, 1);
	}
	this->nrOfLight = nrOfLights;
}
