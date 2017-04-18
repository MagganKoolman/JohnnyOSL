#include "Camera.h"

Camera::Camera()
{
	this->cameraPos = { 0.0, 0.0, 10.0 };
	this->cameraDir = { 0.0, 0.0, -1.0 };
	this->cameraRight = cross(vec3(0.0, 1.0, 0.0), this->cameraDir);

	this->view = glm::lookAt(this->cameraPos,
		this->cameraPos + this->cameraDir,
		glm::vec3(0.0, 1.0, 0.0));
	this->projection = glm::perspective(45.f, (float) SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);
}

Camera::~Camera()
{
}

void Camera::update(double xpos, double ypos, float dt)
{
	dt *= 0.1;
	this->cameraDir +=  this->cameraRight * (float)xpos * dt;
	this->cameraDir.y += (float)ypos * dt;
	this->cameraDir = normalize(cameraDir);
	this->cameraRight = cross(vec3(0.0, 1.0, 0.0), this->cameraDir);
	this->view = glm::lookAt(this->cameraPos,
		this->cameraPos + this->cameraDir,
		glm::vec3(0.0, 1.0, 0.0));	
}

glm::mat4 Camera::getViewProjection()
{
	return this->projection * this->view;
}

void Camera::move(int direction)
{
	switch (direction)
	{
	case 0:
		break;
	case 1:
		this->cameraPos += this->cameraDir * 0.1f;
		break;
	case 2:
		this->cameraPos -= this->cameraDir * 0.1f;
		break;
	case 3:
		this->cameraPos += this->cameraRight * 0.1f;
		break;
	case 4:
		this->cameraPos -= this->cameraRight * 0.1f;
		break;
	}
}

