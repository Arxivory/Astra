#pragma once
#ifndef STAR_H
#define STAR_H

#include <string>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Loader.h"
#include "Constants.h"
#include "CelestialObject.h"

using namespace std;
using namespace glm;

class Star : public CelestialObject {
public:
	Star(string name, float gravity, float mass, float size, float luminosity, float temperature,
		vec3& position, vec3& initialVel, float rotationSpeed);

	void generateSphereMesh();

	void setupShaderProgram();

	void init();

	void setup(const char* texturePath);

	void render(float currentFrame, float timeFactor, const mat4& view,
		const mat4& projection, const vec3& lightPos, const vec3& cameraPos);


private:
	string name;
	float gravity;
	float mass;
	float size;
	float luminosity;
	float temperature;
	float rotationSpeed;
	GLuint textureId;
	vector<float> vertices;
	vector<unsigned int> indices;
	GLuint VAO, VBO, EBO;
	GLuint shaderProgram;
	Loader loader;
};

#endif // !STAR_H

