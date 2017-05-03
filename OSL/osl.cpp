#include "osl.h"
#include <fstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "src\SOIL.h"

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

	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	a.open("textureGen.geom.glsl");
	text = std::string(std::istreambuf_iterator<char>(a), std::istreambuf_iterator<char>());
	textc = text.c_str();
	glShaderSource(geometryShader, 1, &textc, NULL);
	glCompileShader(geometryShader);
	printshaderError(geometryShader);
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
	glAttachShader(textureGenProg, geometryShader);
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
	/*MYrgba *data = new MYrgba[textureRes*textureRes];
	for (int i = 0; i < textureRes*textureRes; i++) {
		data[i] = {255, 0, 0, 255};
	}*/

	createTextures(&cube, "textures/rubik128.png");
	createTextures(&sphere, "textures/daSphere128.png");
	

	glGenTextures(1, &lockTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lockTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, textureRes, textureRes);
	glBindTexture(GL_TEXTURE_2D, 0);


	lights.init(oslprog, nrOfLights);

	/*GLint indexLoc = glGetUniformBlockIndex(oslprog, "Indices");
	GLint blockSize;
	glGetActiveUniformBlockiv(oslprog, indexLoc, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	int size = sizeof(int) * nrOfLights * 4;
	if (blockSize != size)
		printf("Index uniform block and CPU buffer are not same size!!!! Bad!!!\n");

	glGenBuffers(1, &this->indexBuffer);*/
}
void osl::createTextures(oslObject* obj, std::string path) {

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &obj->positionTex);
	glBindTexture(GL_TEXTURE_2D, obj->positionTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, textureRes, textureRes);// , 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &obj->normalTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj->normalTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, textureRes, textureRes);// , 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);

	int width, height, channels;
	unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &channels, 4);

	glGenTextures(1, &obj->diffuseTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj->diffuseTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);// , 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete data;
}

void osl::updateLights(oslInstance* instance, int number)
{

	for (int i = 0; i < number; i++) {
		instance[i].lightsAffecting = 0;
		for (int j = 0; j < nrOfLights; j++) {
			if (instance[i].hb.intersect(&lights.lightboxes[j])) {
				int index = instance[i].lightsAffecting++;
				instance[i].lights[index] = j;
			}
		}
	}
}

void osl::createSphereTextures(int number) {
	//for (int i = 0; i < number; i++) {
	glGenTextures(1, &megaTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, megaTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 4096, 4096);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//}
	nrOfSpheres = number;
}

void osl::createCubeTextures(int number) {
	for (int i = 0; i < number; i++) {
		glGenTextures(1, &cubeInstances[i].tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeInstances[i].tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, textureRes, textureRes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	nrOfCubes = number;
}
int coutnerea = 0;
void osl::render( glm::mat4 vp, glm::vec3 camPos, float dt )
{
	coutnerea++;
	updateLights(sphereInstances, nrOfSpheres);
	updateLights(cubeInstances, nrOfCubes);
	//lights.update(oslprog, dt);
	glUseProgram(oslprog);
	glEnable(GL_TEXTURE_2D);
	//glBindBuffer(GL_UNIFORM_BUFFER, this->indexBuffer);
	setupShading(camPos, &sphere);
	for (int i = 0; i < nrOfSpheres; i++) {
		//if (coutnerea % (2*i+1) == 0)
		if (sphereInstances[i].lightsAffecting > 0) {
			updateShading(sphere, sphereInstances[i], spheres[i], i);
			sphereInstances[i].fixed = false;
		}
		else if (!sphereInstances[i].fixed) {
			updateShading(sphere, sphereInstances[i], spheres[i], i);
			sphereInstances[i].fixed = true;
		}
	}
	setupShading(camPos, &cube);
	for (int i = 0; i < nrOfCubes; i++) {
		//if (coutnerea % (2*i+1) == 0)
		if (cubeInstances[i].lightsAffecting > 0){
			updateShading(cube, cubeInstances[i], cubes[i], 288+i);
			cubeInstances[i].fixed = false;
		}
		else if (!cubeInstances[i].fixed) {
			updateShading(cube, cubeInstances[i], cubes[i], 288 + i);
			cubeInstances[i].fixed = true;
		}
	}
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glUseProgram(0);
	glFinish();
	renderInstances(vp);
}
void osl::renderInstances(glm::mat4 vp) {
	glUseProgram(oslForward);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(sphere.va);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GLuint location = glGetUniformLocation(oslForward, "viewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, &vp[0][0]);
	glActiveTexture(GL_TEXTURE0);
	GLuint texLoc = glGetUniformLocation(oslForward, "diffTex");
	glUniform1i(texLoc, 0);
	location = glGetUniformLocation(oslForward, "world");
	GLuint iLoc = glGetUniformLocation(oslForward, "megaTexIndex");
	glBindTexture(GL_TEXTURE_2D, megaTex);
	for (int i = 0; i < nrOfSpheres; i++) {
		glUniform1i(iLoc, i);
		glUniformMatrix4fv(location, 1, GL_FALSE, &spheres[i][0][0]);

		glDrawArrays(GL_TRIANGLES, 0, sphere.size);
	}

	glBindVertexArray(cube.va);

//	glBindTexture(GL_TEXTURE_2D, megaTex);
	for (int i = 0; i < nrOfCubes; i++) {
		glUniform1i(iLoc, 288+i);
		glUniformMatrix4fv(location, 1, GL_FALSE, &cubes[i][0][0]);

		glDrawArrays(GL_TRIANGLES, 0, cube.size);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void osl::setupShading(glm::vec3 &camPos, oslObject* obj) {

	glBindImageTexture(0, obj->positionTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, obj->normalTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(2, obj->diffuseTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	glBindImageTexture(3, megaTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glUniform3fv(1, 1, &camPos[0]);
}
void osl::updateShading( oslObject &object, oslInstance &instance, glm::mat4 &world, int index) {
	float lightPos[3] = { 1.f, 0.f, 1.5f };
	GLuint location;
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * 10, instance.lights, GL_DYNAMIC_DRAW);

	//GLint indexLoc = glGetUniformLocation(oslprog, "indices");
	glUniform1iv(7, 10, instance.lights);
	//glBindBufferBase(GL_UNIFORM_BUFFER, indexLoc, this->indexBuffer);

	//location = glGetUniformLocation(oslprog, "activeLights");
	glUniform1i(6, instance.lightsAffecting);

	//location = glGetUniformLocation(oslprog, "camPos");
	//location = glGetUniformLocation(oslprog, "world");
	glUniformMatrix4fv(2, 1, GL_FALSE, &world[0][0]);
	//location = glGetUniformLocation(oslprog, "megaTexIndex");
	glUniform1i(5, index);


	glDispatchCompute(textureRes / 8, textureRes / 8, 1);
}
void osl::generateTextures(GLuint va, int size, oslObject object)
{
	glUseProgram(textureGenProg);
	glBindVertexArray(va);
	glEnable(GL_TEXTURE_2D);
	/*GLuint location = glGetUniformLocation(textureGenProg, "positionTex");
	glUniform1i(location, sphere.positionTex);
	location = glGetUniformLocation(textureGenProg, "normalTex");
	glUniform1i(location, sphere.normalTex);*/
	
	glBindImageTexture(0, object.positionTex,	0,	GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindImageTexture(1, object.normalTex,		0,	GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindImageTexture(2, lockTex,				0,	GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

	//GLuint a[2] = { sphere.normalTex, sphere.positionTex };
	//glBindTextures(0, 2, a);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, size);
	glUseProgram(0);
}
osl::osl() {
}

osl::~osl()
{
}
