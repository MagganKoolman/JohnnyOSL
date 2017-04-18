#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>

using namespace glm;

class Camera {
public:
	Camera();
	~Camera();
	void update();
	glm::mat4 getViewProjection();
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 800;

	//Controls
	void move(int direction);
private:
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraRight;
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
};