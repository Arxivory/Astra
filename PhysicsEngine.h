#pragma once
#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "CelestialObject.h"
#include "Constants.h"
#include <vector>

class PhysicsEngine {
public:
	PhysicsEngine();

	~PhysicsEngine();

	void addBody(CelestialObject* body);

	void update(float deltaTime);

	void calculateGravitationalForce(CelestialObject* body1, 
		CelestialObject* body2);

	

private:
	vector<CelestialObject*> bodies;
};

#endif // !PHYSICSENGINE_H

