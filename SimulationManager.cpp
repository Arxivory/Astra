#include "SimulationManager.h"

void SimulationManager::addPlanet(string name, float gravity, float mass, float centralMass, float axialTilt, float distance, float size, float orbitSpeed,
float rotationSpeed, float initialAngle) {
	auto planet = make_unique<Planet>(name, gravity, mass, centralMass, axialTilt, distance, size, orbitSpeed, rotationSpeed, initialAngle);
	physics.addBody(planet.get());
	celestialObjects.push_back(move(planet));
}

vec3 SimulationManager::calculateOrbitalVelocity(const vec3& position, CelestialObject* centralObject) {
	vec3 direction = position - centralObject->getPosition();
	float distance = length(direction);

	float orbitalSpeed = sqrt((G * centralObject->getMass()) / distance);

	vec3 orbitDirection = normalize(cross(vec3(0.0f, 1.0f, 0.0f), direction));
	return orbitDirection * orbitalSpeed;
}

void SimulationManager::update(float deltaTime, float timeFactor) {
	physics.update(deltaTime * timeFactor);
}

void SimulationManager::render(const mat4& view, const mat4& projection, const vec3& lightPos, const vec3& cameraPos) {
	for (auto& celestialObject : celestialObjects)
		celestialObject->render(0.0f, 0.0f, view, projection, lightPos, cameraPos);
}

CelestialObject* SimulationManager::getCelestiaObject(const string& name) {
	for (auto& celestialObject : celestialObjects)
		if (celestialObject->getName() == name)
			return celestialObject.get();

	return nullptr;
}
