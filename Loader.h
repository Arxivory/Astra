#pragma once
#ifndef LOADER_H
#define LOADER_H 


#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Constants.h"

using namespace std;

class Loader {
public:

	GLuint createShaderProgram(const char* vertPath, const char* fragPath);

	GLuint loadTexture(const char* path);

private:

	string loadShaderSource(const char* path);

};

#endif // !LOADER_H
