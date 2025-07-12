#pragma once
#ifndef PLANETMANAGER_H
#define PLANETMANAGER_H
#include <vector>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Planet.h"

using namespace std;
using namespace glm;

class PlanetManager {
private:
	vector<unique_ptr<Planet>> planets;

public:
	PlanetManager();
	virtual ~PlanetManager();

	void addPlanet(const string& name, float gravity, float mass, float centralMass, float axialTilt, float distance, float size,
		float orbitSpeed, float rotationSpeed, float initialAngle, const char* texturePath);

	void renderPlanets(const mat4& view, const mat4& projection, 
		const vec3& lightPos, const vec3& cameraPos, float currentFrame, float timeFactor);
	
	void calculateGravitationalForce(Planet* planet1, Planet* planet2);

	void update(float deltaTime);
};


#endif // !PLANETMANAGER_H
