#include "App.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <set>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar *message, const void *userParam);

App::App() {
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	w = glfwCreateWindow(800, 800, "OSL", NULL, NULL);
	glfwMakeContextCurrent(w);
	int error = glewInit();

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

	sphereSize = 0;
	createSphereVBO(20);
	oslstuff.init();

}
App::~App(){

}
void App::createSphereVBO(int resolution)
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
										(float)j/resolution, //uvs
										(float)i/resolution
			};
		}
	}
	int newRes = resolution;
	std::vector<face> faceData;
	vtxData t = {1,1,1,1,1,1,1,1};
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
		/*int a = i*newRes + newRes;
		int b = i*newRes;
		int c = (i+1)*newRes + newRes;
		int d = (i+1)*newRes;
		int index = 2 * (i*newRes + newRes-2);
		faceData[index] = { data[a],data[b],data[c] }; //winding order(?)
		faceData[index + 1] = { data[b],data[c],data[d] }; //winding order(???)
		sphereSize += 6;*/
	}
	GLuint vbo;
	glGenVertexArrays(1, &sphereVa);
	glBindVertexArray(sphereVa);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(face)* faceData.size(), faceData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vtxData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vtxData), (void*)offsetof(vtxData, x));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vtxData), (void*)offsetof(vtxData, u));

	//delete faceData;
	//delete data;
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
void App::run() {
	glClearColor(1.0, 0.0, 1.0, 1.0);
	while(!glfwWindowShouldClose(w)){
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		oslstuff.render(sphereVa, sphereSize);
		glfwSwapBuffers(w);
		int a = glGetError();
		if (a) {
			std::cout << glewGetErrorString(a) << std::endl;
		}
	}
}
