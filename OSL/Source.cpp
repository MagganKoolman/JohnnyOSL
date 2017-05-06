#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "App.h"
#include <sstream>
using namespace std;
int main(int argc, char* argv[])
{
	bool mode = false;
	int runTime = 10000;
	int shotRate = 9000;
	string destFolder = "";
	if (argc == 5)
	{
		std::stringstream ss(argv[1]);
		ss >> std::boolalpha >> mode;
		runTime = atoi(argv[2]);
		shotRate = atoi(argv[3]);
		destFolder = argv[4];
	}
	App app(mode, runTime, shotRate, destFolder);
	app.run();

	return 0;
}