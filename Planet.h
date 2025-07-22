#pragma once
#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Loader.h"
#include "CelestialObject.h"

using namespace std;
using namespace glm;

class Planet : public CelestialObject {
public:
	Planet(string name, float gravity, float mass, float centralMass, float axialTilt, vec3& initialPosition, float size, vec3& initialVelocity,
		float rotationSpeed, float initialAngle);

	void generateSphereMesh();

	void setupShaderProgram();

	void init();

	void setup(const char *texturePath);

	void render(float currentFrame, float timeFactor, const mat4& view, 
		const mat4& projection, const vec3& lightPos, const vec3& cameraPos);

	float getSize() { return size; }
	float getMass() { return mass; }

private:
	
	float gravity;
	float mass;
	float centralMass;
	float axialTilt;
	float distance;
	float size;
	float orbitSpeed;
	float rotationSpeed;
	float initialAngle;
	GLuint textureId;
	vector<float> vertices;
	vector<unsigned int> indices;
	GLuint VBO, VAO, EBO;
	GLuint shaderProgram;
	Loader loader;
	

};

#endif // !PLANET_H
