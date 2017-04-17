#include "osl.h"
#include <fstream>
#include <vector>

void printshaderError(GLuint shader) {
	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shader);
		std::printf("%s\n", &(errorLog[0]));
	}
}
void osl::init() {
	const char* textc;

	GLuint oslshader = glCreateShader(GL_COMPUTE_SHADER);
	std::ifstream a("oslShader.comp.glsl");
	std::string text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	textc = text.c_str();
	glShaderSource(oslshader, 1, &textc, NULL);	
	glCompileShader(oslshader);
	printshaderError(oslshader);

	oslprog = glCreateProgram();
	glAttachShader(oslprog, oslshader);
	glLinkProgram(oslprog);
	a.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	a.open("textureGen.vert.glsl");
	text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	textc = text.c_str();
	glShaderSource(vertexShader, 1, &textc, NULL);
	glCompileShader(vertexShader);
	printshaderError(vertexShader);
	a.close();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	a.open("textureGen.frag.glsl");
	text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	textc = text.c_str();
	glShaderSource(fragmentShader, 1, &textc, NULL);
	glCompileShader(fragmentShader);
	printshaderError(fragmentShader);

	textureGenProg = glCreateProgram();
	glAttachShader(textureGenProg, vertexShader);
	glAttachShader(textureGenProg, fragmentShader);
	glLinkProgram(textureGenProg);


	//textures

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &sphere.positionTex);
	glBindTexture(GL_TEXTURE_2D, sphere.positionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureRes, textureRes, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &sphere.normalTex);
	glBindTexture(GL_TEXTURE_2D, sphere.normalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureRes, textureRes, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture(GL_TEXTURE_2D, 0);



}
void osl::render(GLuint va, int size)
{
	glUseProgram(oslprog);
	glBindVertexArray(va);

	GLuint location = glGetUniformLocation(oslprog, "positionTex");
	glUniform1i(location, sphere.positionTex);
	location = glGetUniformLocation(oslprog, "normalTex");
	glUniform1i(location, sphere.normalTex);
	location = glGetUniformLocation(oslprog, "destTex");
	glUniform1i(location, sphere.diffuseTex);
	float lightPos[3] = { 0.f, 10.f, 0.f };
	float camPos[3] = { 0.f,0.f, -10.f};
	location = glGetUniformLocation(oslprog, "lightPos");
	glUniform3fv(location, 1, &lightPos[0]);
	location = glGetUniformLocation(oslprog, "camPos");
	glUniform3fv(location, 1, &camPos[0]);

	/*GLuint a[3] = { sphere.normalTex, sphere.positionTex, sphere.diffuseTex };
	glBindTextures(0, 3, a);*/

	glBindImageTexture(0, sphere.positionTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGB32F);
	glBindImageTexture(1, sphere.normalTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGB32F);
	glBindImageTexture(2, sphere.diffuseTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);


	glDrawArrays(GL_TRIANGLES, 0, size);
}
void osl::generateTextures(GLuint va, int size)
{
	glUseProgram(textureGenProg);
	glBindVertexArray(va);
	GLuint location = glGetUniformLocation(textureGenProg, "positionTex");
	glUniform1i(location, sphere.positionTex);
	location = glGetUniformLocation(textureGenProg, "normalTex");
	glUniform1i(location, sphere.normalTex);
	GLuint a[2] = { sphere.normalTex, sphere.positionTex };
	glBindTextures(0, 2, a);

	glDrawArrays(GL_TRIANGLES, 0, size);
}
osl::osl() {
}

osl::~osl()
{
}
