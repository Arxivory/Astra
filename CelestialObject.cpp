#include "CelestialObject.h"

CelestialObject::CelestialObject(string name, float mass, float radius, vec3 initialPosition, 
	vec3 initialVelocity) :
	name(name),
	mass(mass),
	radius(radius),
	position(initialPosition),
	velocity(initialVelocity),
	force(0.0f) { }

void CelestialObject::addForce(const vec3& f) {
	force += f;
}

void CelestialObject::clearForces() {
	force = vec3(0.0f);
}

void CelestialObject::updatePhysics(float deltaTime) {
	vec3 acceleration = force / mass;

	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;

	clearForces();
}