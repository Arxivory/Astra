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

	float forceMagnitude = G * body1->getMass() * body2->getMass() / (distance * distance);
	vec3 force = forceMagnitude * direction;
	body1->addForce(force);
	body2->addForce(-force);
}

void PhysicsEngine::update(float deltaTime) {
	for (auto* body : bodies) 
		body->clearForces();

	for (int i = 0; i < bodies.size(); i++) 
		for (int j = i + 1; j < bodies.size(); j++) 
			calculateGravitationalForce(bodies[i], bodies[j]);

	for (auto* body : bodies)
		body->updatePhysics(deltaTime);
}