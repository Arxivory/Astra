#include "PlanetManager.h"
#include <algorithm>

PlanetManager::PlanetManager() {}

PlanetManager::~PlanetManager() {
	planets.clear();
}

void PlanetManager::addPlanet(const string& name, float gravity, float mass, float centralMass, float axialTilt, float distance, float size,
	float orbitSpeed, float rotationSpeed, float initialAngle, const char* texturePath) {

	auto planet = make_unique<Planet>(name, gravity, mass, centralMass, axialTilt, distance, size, orbitSpeed, rotationSpeed, initialAngle);
	planet->init();
	planet->setup(texturePath);
	planets.push_back(move(planet));

}

void PlanetManager::calculateGravitationalForce(Planet* planet1, Planet* planet2) {
	vec3 direction = planet2->getPosition() - planet1->getPosition();
	float distance = length(direction);

	if (distance < (planet1->getSize() + planet2->getSize()) * 2.0f)
		distance = (planet1->getSize() + planet2->getSize()) * 2.0f;

	direction = normalize(direction);

	float forceMagnitude = (G * planet1->getMass() * planet2->getMass()) / (distance * distance);

	vec3 force = direction * forceMagnitude;
	planet1->addForce(force);
	planet2->addForce(-force); 
}

void PlanetManager::update(float deltaTime) {
	for (auto& planet : planets) {
		if (planet) {
			planet->clearForces();
		}
		else {
			cerr << "Error: Planet is Null" << endl;
		}
	}
	for (size_t i = 0; i < planets.size(); ++i) {
		for (size_t j = i + 1; j < planets.size(); ++j) {
			if (planets[i] && planets[j]) {
				calculateGravitationalForce(planets[i].get(), planets[j].get());
			}
		}
	}
	for (auto& planet : planets) {
		if (planet) {
			planet->updatePhysics(deltaTime);
		}
		else {
			cerr << "Error: Planet is Null" << endl;
		}
	}
}

void PlanetManager::renderPlanets(const mat4& view, const mat4& projection, 
	const vec3& lightPos, const vec3& cameraPos, float currentFrame, float timeFactor) {
	update(1.0f * timeFactor);
	for (const auto& planet : planets) {
		if (planet) {
			planet->render(currentFrame, timeFactor, view, projection, lightPos, cameraPos);
		}
		else {
			cerr << "Error: Planet is Null" << endl;
		}
	}
}