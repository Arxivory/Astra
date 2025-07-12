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

using namespace std;
using namespace glm;

class Planet {
public:
	Planet(string name, float gravity, float mass, float centralMass, float axialTilt, float distance, float size, float orbitSpeed,
		float rotationSpeed, float initialAngle);

	void generateSphereMesh();

	void setupShaderProgram();

	void init();

	void setup(const char *texturePath);

	void render(float currentFrame, float timeFactor, const mat4& view, 
		const mat4& projection, const vec3& lightPos, const vec3& cameraPos);

	string getName() { return name; }

	vec3 getPosition() { return position; }
	float getSize() { return size; }
	float getMass() { return mass; }
	void addForce(vec3 force) { this->force += force; }
	void updatePhysics(float deltaTime);
	void clearForces() { this->force = vec3(0.0f); }

private:
	vec3 position;
	vec3 velocity;
	vec3 acceleration;
	vec3 force;

	string name;
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

	void setupPosition();
};

#endif // !PLANET_H
