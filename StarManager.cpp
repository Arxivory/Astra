#include "StarManager.h"
#include <algorithm>

StarManager::StarManager() {}

StarManager::~StarManager() {
	stars.clear();
}

void StarManager::addStar(string name, float gravity, float mass, float size, float luminosity, float temperature,
	vec3& position, float rotationSpeed, const char* texturePath) {

	auto star = make_unique<Star>(name, gravity, mass, size, luminosity, temperature, position, rotationSpeed);
	star->init();
	star->setup(texturePath);
	stars.push_back(move(star));
}

void StarManager::renderStars(const mat4& view, const mat4& projection, const vec3& lightPos,
	const vec3& cameraPos, float currentFrame, float timeFactor) {
	for (const auto& star : stars) {
		if (star)
			star->render(currentFrame, timeFactor, view, projection, lightPos, cameraPos);
		else
			cout << "Star is Null" << endl;
	}
}