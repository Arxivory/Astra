#pragma once
#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class CelestialObject {
public:
	CelestialObject(string name, float mass, float radius, vec3 initialPosition, 
		vec3 initialVelocity);

	virtual ~CelestialObject() = default;

	void addForce(vec3 &f);

	void clearForces();

	void updatePhysics(float deltaTime);

	string getName() { return name; }
	float getMass() { return mass; }
	float getRadius() { return radius; }
	vec3 getPosition() { return position; }
	vec3 getVelocity() { return velocity; }
	vec3 getForce() { return force; }

	void setName(string name) { this->name = name; }
	void setMass(float mass) { this->mass = mass; }
	void setRadius(float radius) { this->radius = radius; }
	void setPosition(vec3 position) { this->position = position; }
	void setVelocity(vec3 velocity) { this->velocity = velocity; }
	void setForce(vec3 force) { this->force = force; }

	virtual void render(float currentFrame, float timeFactor, const mat4& view,
		const mat4& projection, const vec3& lightPos, const vec3& cameraPos) = 0;

private:
	string name;
	float mass;
	float radius;

	vec3 position;
	vec3 velocity;
	vec3 force;
};

#endif // !CELESTIALOBJECT_H

