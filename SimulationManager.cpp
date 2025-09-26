#include "SimulationManager.h"

void SimulationManager::addPlanet(string name, float gravity, float mass, float centralMass, float axialTilt, vec3& initialPosition, float size, vec3& initialVelocity,
	float rotationSpeed, float initialAngle, const char* texturePath) {
	auto planet = make_unique<Planet>(name, gravity, mass, centralMass, axialTilt, initialPosition, size, initialVelocity, rotationSpeed, initialAngle);
	planet->init();
	planet->setup(texturePath);
	physics.addBody(planet.get());
	celestialObjects.push_back(move(planet));
}

void SimulationManager::addStar(string name, float gravity, float mass, float size, float luminosity, float temperature,
	vec3& position, vec3& initialVel, float rotationSpeed, const char* texturePath) {
	auto star = make_unique<Star>(name, gravity, mass, size, luminosity, temperature, position, initialVel, rotationSpeed);
	star->init();
	star->setup(texturePath);
	physics.addBody(star.get());
	celestialObjects.push_back(move(star));
}

vec3 SimulationManager::calculateOrbitalVelocity(const vec3& position, CelestialObject* centralObject) {
	vec3 direction = position - centralObject->getPosition();
	float distance = length(direction);

	float orbitalSpeed = sqrt((G_NORMALIZED * centralObject->getMass()) / distance);

	vec3 orbitDirection = normalize(cross(vec3(0.0f, 1.0f, 0.0f), direction));
	return orbitDirection * orbitalSpeed;
}

void SimulationManager::update(float deltaTime, float timeFactor) {
	physics.update(deltaTime * timeFactor);
}

void SimulationManager::render(const mat4& view, const mat4& projection, const vec3& lightPos, const vec3& cameraPos, float timeFactor) {
	for (auto& celestialObject : celestialObjects) {
		if (trajectoryVisible) {
			celestialObject->updateTrajectory();
			celestialObject->renderTrajectory(view, projection);
		}
		celestialObject->render(0.0f, timeFactor, view, projection, lightPos, cameraPos);
		
	}
}

CelestialObject* SimulationManager::getCelestialObject(const string& name) {
	for (auto& celestialObject : celestialObjects)
		if (celestialObject->getName() == name)
			return celestialObject.get();

	return nullptr;
}
