#pragma once
#ifndef STARMANAGER_H
#define STARMANAGER_H

#include <vector>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Star.h"

using namespace std;
using namespace glm;

class StarManager {
private:
	vector<unique_ptr<Star>> stars;

public:
	StarManager();
	virtual ~StarManager();

	void addStar(string name, float gravity, float mass, float size, float luminosity, float temperature,
		vec3& position, float rotationSpeed, const char* texturePath);

	void renderStars(const mat4& view, const mat4& projection, const vec3& lightPos,
		const vec3& cameraPos, float currentFrame, float timeFactor);
};

#endif // !STARMANAGER_H

