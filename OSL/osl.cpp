#include "osl.h"
#include <fstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

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
void printprogramError(GLuint program) {
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
		std::printf("%s\n", &(errorLog[0]));
	}
}
void osl::init() {
	const char* textc;
	//compute shader
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
	printprogramError(oslprog);
	a.close();

	//texure gen shader
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
	a.close();

	textureGenProg = glCreateProgram();
	glAttachShader(textureGenProg, vertexShader);
	glAttachShader(textureGenProg, fragmentShader);
	glLinkProgram(textureGenProg);

	//osl forawrd shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	a.open("oslForward.vert.glsl");
	text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	textc = text.c_str();
	glShaderSource(vertexShader, 1, &textc, NULL);
	glCompileShader(vertexShader);
	printshaderError(vertexShader);
	a.close();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	a.open("oslForward.frag.glsl");
	text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	textc = text.c_str();
	glShaderSource(fragmentShader, 1, &textc, NULL);
	glCompileShader(fragmentShader);
	printshaderError(fragmentShader);
	a.close();

	oslForward = glCreateProgram();
	glAttachShader(oslForward, vertexShader);
	glAttachShader(oslForward, fragmentShader);
	glLinkProgram(oslForward);

	//textures
	struct MYrgba {
		unsigned char r, g, b, a;
	};
	MYrgba *data = new MYrgba[textureRes*textureRes];
	for (int i = 0; i < textureRes*textureRes; i++) {
		data[i] = {255, 0, 0, 255};
	}


	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &sphere.positionTex);
	glBindTexture(GL_TEXTURE_2D, sphere.positionTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, textureRes, textureRes);// , 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &sphere.normalTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphere.normalTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, textureRes, textureRes);// , 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &sphere.diffuseTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphere.diffuseTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, textureRes, textureRes);// , 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureRes, textureRes, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &sphereTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphereTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, textureRes, textureRes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete data;
}
void osl::render(GLuint va, int size, glm::mat4 vp)
{
	glUseProgram(oslprog);
	glEnable(GL_TEXTURE_2D);
	GLuint location;
	float lightPos[3] = { 0.f, 10.f, 0.f };
	float camPos[3] = { 0.f,0.f, 10.f};
	location = glGetUniformLocation(oslprog, "lightPos");
	glUniform3fv(location, 1, &lightPos[0]);
	location = glGetUniformLocation(oslprog, "camPos");
	glUniform3fv(location, 1, &camPos[0]);

	glBindImageTexture(0, sphere.positionTex,	0,	GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, sphere.normalTex,		0,	GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(2, sphere.diffuseTex,	0,	GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(3, sphereTex,			0,	GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


	glDispatchCompute(16,16,1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glUseProgram(0);

	glFinish();


	glUseProgram(oslForward);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(va);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	location = glGetUniformLocation(oslForward, "viewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, &vp[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphereTex);
	location = glGetUniformLocation(oslForward, "diffTex");
	glUniform1i(location, 0);

	glDrawArrays(GL_TRIANGLES, 0, size);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
void osl::generateTextures(GLuint va, int size)
{
	glUseProgram(textureGenProg);
	glBindVertexArray(va);
	glEnable(GL_TEXTURE_2D);
	/*GLuint location = glGetUniformLocation(textureGenProg, "positionTex");
	glUniform1i(location, sphere.positionTex);
	location = glGetUniformLocation(textureGenProg, "normalTex");
	glUniform1i(location, sphere.normalTex);*/
	
	glBindImageTexture(0, sphere.positionTex,	0,	GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindImageTexture(1, sphere.normalTex,		0,	GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	
	//GLuint a[2] = { sphere.normalTex, sphere.positionTex };
	//glBindTextures(0, 2, a);

	glDrawArrays(GL_TRIANGLES, 0, size);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glUseProgram(0);
}
osl::osl() {
}

osl::~osl()
{
}
