#pragma once
#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include "CelestialObject.h"
#include "PhysicsEngine.h"
#include "Planet.h"
#include "Star.h"
#include <iostream>
#include <memory>
#include <vector>
#include "Constants.h"

class SimulationManager {
private:
	vector<unique_ptr<CelestialObject>> celestialObjects;
	PhysicsEngine physics;

public:
	void addPlanet(string name, float gravity, float mass, float centralMass, float axialTilt, vec3& initialPosition, float size, vec3& initialVelocity,
		float rotationSpeed, float initialAngle, const char* texturePath);

	void addStar(string name, float gravity, float mass, float size, float luminosity, float temperature,
		vec3& position, vec3& initialVel, float rotationSpeed, const char* texturePath);

	vec3 calculateOrbitalVelocity(const vec3& position, CelestialObject* centralObject);

	void update(float deltaTime, float timeFactor);

	void render(const mat4& view, const mat4& projection, const vec3& lightPos, const vec3& cameraPos, float timeFactor);

	CelestialObject* getCelestialObject(const string& name);

};

#endif // !SIMULATIONMANAGER_H

