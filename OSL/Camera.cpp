#include "Camera.h"

Camera::Camera()
{
	this->cameraPos = { 0.0, 0.0, 10.0 };
	this->cameraFront = { 0.0, 0.0, -1.0 };
	this->cameraRight = cross(vec3(0.0, 1.0, 0.0), this->cameraFront);

	this->view = glm::lookAt(this->cameraPos,
		this->cameraPos + this->cameraFront,
		glm::vec3(0.0, 1.0, 0.0));
	this->projection = glm::perspective(45.f, (float) SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);
}

Camera::~Camera()
{
}


void Camera::update()
{
	this->view = glm::lookAt(this->cameraPos,
		this->cameraPos + this->cameraFront,
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
		this->cameraPos += this->cameraFront * 0.1f;
		break;
	case 2:
		this->cameraPos -= this->cameraFront * 0.1f;
		break;
	case 3:
		this->cameraPos += this->cameraRight * 0.1f;
		break;
	case 4:
		this->cameraPos -= this->cameraRight * 0.1f;
		break;
	}
}
