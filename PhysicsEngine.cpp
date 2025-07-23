#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {}

PhysicsEngine::~PhysicsEngine() {}

void PhysicsEngine::addBody(CelestialObject* body) {
	bodies.push_back(body);
}

void PhysicsEngine::calculateGravitationalForce(CelestialObject* body1, 
	CelestialObject* body2) {
	vec3 direction = body2->getPosition() - body1->getPosition();
	float distance = length(direction);

	if (distance < body1->getRadius() + body2->getRadius()) 
		distance = body1->getRadius() + body2->getRadius();
	
	direction = normalize(direction);

	float forceMagnitude = G_NORMALIZED * body1->getMass() * body2->getMass() / (distance * distance);
	vec3 force = forceMagnitude * direction;
	body1->addForce(force);
	body2->addForce(-force);
}

void PhysicsEngine::handleCollisions(CelestialObject* body1, CelestialObject* body2) {
	vec3 direction = body2->getPosition() - body1->getPosition();
	float distance = length(direction);
	float minDistance = body1->getRadius() + body2->getRadius();

	if (distance < minDistance) {
		direction = normalize(direction);
		float overlap = minDistance - distance;
		vec3 separation = direction * (overlap * 0.5f);

		vec3 pos1 = body1->getPosition() - separation;
		vec3 pos2 = body2->getPosition() + separation;
		body1->setPosition(pos1);
		body2->setPosition(pos2);
	}
}

void PhysicsEngine::update(float deltaTime) {
	for (auto* body : bodies) 
		body->clearForces();

	for (int i = 0; i < bodies.size(); i++) 
		for (int j = i + 1; j < bodies.size(); j++) {
			calculateGravitationalForce(bodies[i], bodies[j]);
			handleCollisions(bodies[i], bodies[j]);
		}

	for (auto* body : bodies)
		body->updatePhysics(deltaTime);
}