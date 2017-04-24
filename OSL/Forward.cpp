#include "Forward.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
void checkShaderIV(GLuint shader)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar* errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shader);
		cout << errorLog << endl;
		delete errorLog;
		return;
	}
}

bool checkProgramIV(GLuint program)
{
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar* errorLog = new GLchar[maxLength];
		glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
		glDeleteProgram(program);
		cout << errorLog << endl;
		return false;
	}
	return true;
}

Forward::Forward()
{
}

Forward::~Forward()
{
}

void Forward::init()
{
	//Create shaders
	const char* textc;
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream a("forward.vert.glsl");
	std::string text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	a.close();
	textc = text.c_str();
	glShaderSource(vertex, 1, &textc, NULL);
	glCompileShader(vertex);
	checkShaderIV(vertex);

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	a.open("forward.frag.glsl");
	text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	a.close();
	textc = text.c_str();
	glShaderSource(fragment, 1, &textc, NULL);
	glCompileShader(fragment);
	checkShaderIV(fragment);

	//Create shader program
	this->programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);

	if (checkProgramIV(programID))
	{
		glDetachShader(programID, vertex);
		glDetachShader(programID, fragment);
	}
	else {
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	int x = glGetError();
	if (x != 0)
		cout << "Error in forward program constructor, nr: " << x << endl;

	lights.init(this->programID, 2);
}

void Forward::render(glm::mat4 view, glm::mat4 viewProj, glm::vec3 position)
{
	glUseProgram(this->programID);

	GLint loc = glGetUniformLocation(this->programID, "viewProjection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &viewProj[0][0]);
	
	GLint loc2 = glGetUniformLocation(this->programID, "cameraPos");
	glUniform3fv(loc2, 1, &position[0]);
	
	GLint locWorld = glGetUniformLocation(this->programID, "world");

	lights.update(this->programID, 0.01f);

	glBindVertexArray(cubeVao);
	glBindTexture(GL_TEXTURE_2D, cubeTex);
	for (int i = 0; i < 25*25; i++)
	{
		glUniformMatrix4fv(locWorld, 1, GL_FALSE, &cubes[i][0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	glBindVertexArray(sphereVao);
	glBindTexture(GL_TEXTURE_2D, sphereTex);
	for (int i = 0; i < 25*25; i++)
	{
		glUniformMatrix4fv(locWorld, 1, GL_FALSE, &spheres[i][0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 4800);
	}
}
