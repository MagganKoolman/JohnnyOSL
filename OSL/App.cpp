#include "App.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar *message, const void *userParam);

glm::vec3 movement = { 0,0,0 };
bool wDown, sDown, aDown, dDown, eDown, qDown;
bool running;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_W)
			wDown = true;
		if (key == GLFW_KEY_S)
			sDown = true;
		if (key == GLFW_KEY_A)
			aDown = true;
		if (key == GLFW_KEY_D)
			dDown = true;
		if (key == GLFW_KEY_Q)
			qDown = true;
		if (key == GLFW_KEY_E)
			eDown = true;
		if (key == GLFW_KEY_ESCAPE)
			running = false;
	}
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_W)
			wDown = false;
		if (key == GLFW_KEY_S)
			sDown = false;
		if (key == GLFW_KEY_A)
			aDown = false;
		if (key == GLFW_KEY_D)
			dDown = false;
		if (key == GLFW_KEY_Q)
			qDown = false;
		if (key == GLFW_KEY_E)
			eDown = false;
	}
}
void App::updateInputs() {
	movement = {  aDown + -1 *dDown, eDown + -1 * qDown, wDown + -1 * sDown };
}
App::App() {
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	w = glfwCreateWindow(Camera::SCREEN_WIDTH, Camera::SCREEN_HEIGHT, "OSL", NULL, NULL);
	glfwMakeContextCurrent(w);
	int error = glewInit();
	running = true;
	if (error)
		std::cout << ((char*)glewGetErrorString(error));
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(gl_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE,
		GL_DONT_CARE,
		GL_DONT_CARE,
		0,
		nullptr,
		GL_TRUE
		);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(w, key_callback);
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//oslstuff.init();

	sphereSize = 0;
	forwardProgram.sphereVao = createSphereVBO(40);
	createSpheres();
	forwardProgram.spheres = sphereMatrices;

	forwardProgram.cubeVao = createCubeVBO();
	createCubes();
	forwardProgram.cubes = cubeMatrices;
	
	forwardProgram.init();
}
App::~App(){

}


GLuint App::createSphereVBO(int resolution)
{
	float increment = (3.1415 * 2)/resolution;
	std::vector<vtxData> data;
	data.resize(resolution*(resolution/2+1));
	//vtxData* data = new vtxData[];
	float x, y, z;
	for (int i = 0; i < resolution/2+1; i++)
	{
		for (int j = 0; j < resolution; j++) {
			x = sinf(j*increment)*sinf(i*increment);
			y = cosf(i*increment);
			z = cosf(j*increment)*sinf(i*increment);
			data[i*resolution + j] = {	x/2, //positions
										y/2, 
										z/2,
										x,//normals
										y,
										z,
										(float)j/(resolution-1), //uvs
										(float)i/(resolution/2)
			};
		}
	}
	int newRes = resolution;
	std::vector<face> faceData;
	vtxData t = { 1,1,1,1,1,1,1,1 };
	faceData.resize(2 * newRes*(newRes / 2 + 1) + 10, {t,t,t});
	//face* faceData = new face[2*newRes*(newRes/2+1)];
	for (int i = 0; i < newRes/2; i++) {
		for (int j = 0; j < newRes; j++) {
			int a = i*newRes + j;
			int b = i*newRes + (j + 1) % newRes;
			int c = (i + 1)*newRes + j;
			int d = (i + 1)*newRes + (j + 1) % newRes;
			int index = 2 * (i*newRes + j);
			faceData[index] = { data[a],data[b],data[c] }; //winding order(?)
			faceData[index + 1] = { data[b],data[c],data[d] }; //winding order(???)
			sphereSize += 6;
		}
	}
	GLuint vbo;
	glGenVertexArrays(1, &sphereVa);
	glBindVertexArray(sphereVa);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(face)* faceData.size(), faceData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vtxData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vtxData), (void*)offsetof(vtxData, x));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vtxData), (void*)offsetof(vtxData, u));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	forwardProgram.sphereTex = loadTexture("textures/daSphere.png");
	return sphereVa;
}


GLuint App::createCubeVBO()
{
	GLuint vbo;
	glGenVertexArrays(1, &cubeVa);
	glBindVertexArray(cubeVa);
	//Vertices
	glm::vec3 pos[8] = { {0.5, 0.5, 0.5},
						{0.5, 0.5, -0.5},
						{0.5, -0.5, 0.5},
						{0.5, -0.5, -0.5},
						{-0.5, 0.5, 0.5},
						{-0.5, 0.5, -0.5},
						{-0.5, -0.5, 0.5},
						{-0.5, -0.5, -0.5}
	};
	//Normals
	glm::vec3 norm[6] =
	{
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 0.0, -1.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 }
	};
	//UV coordinates
	glm::vec2 UVs[12] = { {0, 0 } ,    //0
						{ 0.5, 0 } ,   //1
						{ 1, 0 } ,	   //2
						{ 0, 1.f/3.f },	   //3
						{ 0.5, 1.f/3.f },  //4
						{ 1, 1.f / 3.f },	   //5
						{ 0, 2.f/3.f },	   //6
						{ 0.5, 2.f/3.f },  //7
						{ 1, 2.f/3.f },	   //8
						{ 0, 1 },	   //9
						{ 0.5, 1 },	   //10
						{ 1, 1 }	   //11
	};
	//Faces
	betterFace faces[12] = {{ { pos[0], norm[4], UVs[10] }, { pos[1], norm[4], UVs[11] }, { pos[2], norm[4], UVs[7] } }, 
							{ { pos[1], norm[4], UVs[11] }, { pos[3], norm[4], UVs[8] }, { pos[2], norm[4], UVs[7] } },
											  																   
							{ { pos[1], norm[1], UVs[5] }, { pos[0], norm[1], UVs[2] }, { pos[4], norm[1], UVs[1] } } , 
							{ { pos[1], norm[1], UVs[5] }, { pos[4], norm[1], UVs[1] }, { pos[5], norm[1], UVs[4] } } ,
							 								 				 			 					   
							{ { pos[4], norm[5], UVs[8] }, { pos[7], norm[5], UVs[4] }, { pos[5], norm[5], UVs[7] } } , 
							{ { pos[4], norm[5], UVs[8] }, { pos[6], norm[5], UVs[5] }, { pos[7], norm[5], UVs[4] } } ,
							 								 				 			 					   
							{ { pos[2], norm[3], UVs[7] }, { pos[3], norm[3], UVs[4] }, { pos[6], norm[3], UVs[6] } } , 
							{ { pos[3], norm[3], UVs[4] }, { pos[7], norm[3], UVs[3] }, { pos[6], norm[3], UVs[6] } } ,
							 								 				 			 					   
							{ { pos[0], norm[2], UVs[10] }, { pos[6], norm[2], UVs[6] }, { pos[2], norm[2], UVs[7] } } , 
							{ { pos[0], norm[2], UVs[10] }, { pos[4], norm[2], UVs[9] }, { pos[6], norm[2], UVs[6] } } ,
														 			 				 						   
							{ { pos[1], norm[0], UVs[1] }, { pos[5], norm[0], UVs[0] }, { pos[7], norm[0], UVs[3] } } , 
							{ { pos[1], norm[0], UVs[1] }, { pos[7], norm[0], UVs[3] }, { pos[3], norm[0], UVs[4] } }
	
	};			

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(betterFace) * 12, &faces, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(betterData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(betterData), (void*)offsetof(betterData, normals));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(betterData), (void*)offsetof(betterData, UV));

	forwardProgram.cubeTex = loadTexture("textures/daSphere2.png");
	return cubeVa;
}

void App::createCubes()
{
	int index = 0;
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++) {
			cubeMatrices[index++] = glm::translate(glm::mat4(1), glm::vec3(i *2* 2, -2, j*2*2));
		}
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++) {
			cubeMatrices[index++] = glm::translate(glm::mat4(1), glm::vec3(i *2* 2, 2, (j*2+1) * 2));
		}
	}
}

void App::createSpheres()
{
	int index = 0;
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j ++) {
			sphereMatrices[index++] = glm::translate(glm::mat4(1), glm::vec3(i*2*2, -2, (j*2+1)*2));
		}
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j ++) {
			sphereMatrices[index++] = glm::translate(glm::mat4(1), glm::vec3(i * 2*2, 2, j *2* 2));
		}
	}
}

GLuint App::loadTexture(std::string path)
{
	GLuint tex = SOIL_load_OGL_texture
	(
		path.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, 
		SOIL_FLAG_INVERT_Y
	);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

void App::run() {
	glClearColor(1.0, 0.0, 1.0, 1.0);
	double xpos, ypos, lastx, lasty;
	glfwGetCursorPos(w, &lastx, &lasty);
	double time, dt;
	time = 0.0;
	glfwSetTime(time);
	oslstuff.generateTextures(sphereVa, sphereSize);
	while(!glfwWindowShouldClose(w) && running){
		dt = glfwGetTime() - time;	
		time = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();	
		glfwGetCursorPos(w, &xpos, &ypos);
		camera.update(lastx - xpos, lasty - ypos, dt);
		lastx = xpos;
		lasty = ypos;
		updateInputs();
		camera.move(movement, dt);
		//oslstuff.render(sphereVa, sphereSize, camera.getViewProjection());
		forwardProgram.render(camera.view, camera.getViewProjection(), camera.cameraPos);
		glfwSwapBuffers(w);
		int a = glGetError();
		if (a) {
			std::cout << glewGetErrorString(a) << std::endl;
		}
	}
}

void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar *message, const void *userParam)
{
	static std::set<GLuint> SHOWED_ID;

	if (type != GL_DEBUG_TYPE_ERROR) {
		if (SHOWED_ID.insert(id).second == false) return;
	}

	const char *stype = "",
		*sseverity = "",
		*ssource = "";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		stype = "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		stype = "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		stype = "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		stype = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		stype = "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		stype = "OTHER";
		break;
	case GL_DEBUG_TYPE_MARKER:
		stype = "MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		stype = "PUSH_GROUP";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		stype = "POP_GROUP";
		break;
	default:
		stype = "UNKNOWN";
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		sseverity = "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		sseverity = "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		sseverity = "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		sseverity = "NOTIFICATION";
		break;
	default:
		sseverity = "UNKNOWN";
	}
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		ssource = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		ssource = "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		ssource = "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		ssource = "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		ssource = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		ssource = "OTHER";
		break;
	default:
		ssource = "UNKNOWN";
	}
	printf("[OpenGL] [%i:%s] [%i:%s] [%i:%s] %u: %*s\n",
		(int)severity, sseverity,
		(int)type, stype,
		(int)source, ssource,
		id, length, message
	);
}
