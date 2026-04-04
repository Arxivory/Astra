#include "Planet.h"

using namespace std;


Planet::Planet(string name, float gravity, float mass, float centralMass, float axialTilt, vec3& initialPosition, float size,
	vec3& initialVelocity, float rotationSpeed, float initialAngle) : CelestialObject(name, mass, size* SCALE_FACTOR, initialPosition, initialVelocity),
	gravity(gravity),
	mass(mass* SCALE_FACTOR),
	centralMass(centralMass* SCALE_FACTOR),
	axialTilt(axialTilt),
	distance(distance* SCALE_FACTOR),
	size(size* SCALE_FACTOR),
	orbitSpeed(orbitSpeed),
	rotationSpeed(rotationSpeed),
	initialAngle(initialAngle),
	VAO(0), VBO(0), EBO(0), textureId(0) {
}


void Planet::setupShaderProgram() {
	shaderProgram = loader.createShaderProgram("shaders/vertex/PlanetVertexShader.glsl", "shaders/fragment/PlanetFragmentShader.glsl");

	if (shaderProgram == 0) {
		cerr << "Error creating Shader Program for Planet: " << name << endl;
		return;
	}

	cout << "Planet Shader Program created successfully for: " << name << endl;
}

void Planet::init() {
	setupShaderProgram();
	generateSphereMesh();
	setupTrajectoryShaders();
	setupTrajectoryBuffers();
}

void Planet::generateSphereMesh() {
	int sectorCount = 128, stackCount = sectorCount;
	const float PI = 3.14159265359f;
	vertices.clear();
	indices.clear();

	float x, y, z, xy;
	float s, t;
	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i) {
		stackAngle = PI / 2 - i * stackStep;
		xy = cosf(stackAngle);
		z = sinf(stackAngle);

		for (int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);

			s = (float)j / sectorCount;
			t = 1.0f - (float)i / stackCount;

			// 1. Position (3 floats)
			vertices.push_back(x);
			vertices.push_back(z);
			vertices.push_back(y);

			// 2. TexCoords (2 floats)
			vertices.push_back(s);
			vertices.push_back(t);

			// 3. Tangent (3 floats) - NEW
			// For a sphere, the tangent is the derivative with respect to the sector angle
			vertices.push_back(-sinf(sectorAngle));
			vertices.push_back(0.0f);
			vertices.push_back(cosf(sectorAngle));
		}
	}

	int k1, k2;
	for (int i = 0; i < stackCount; ++i) {
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}

void Planet::setup(const char* texturePath, const char* normalPath) {

	textureId = loader.loadTexture(texturePath, true);
	normalMapId = loader.loadTexture(normalPath, true);

	if (textureId == 0) {
		cerr << "Error loading texture for Planet: " << name << endl;
		return;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	int stride = 8 * sizeof(float);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}



void Planet::render(float currentFrame, float timeFactor, const mat4& view,
	const mat4& projection, const vec3& lightPos, const vec3& cameraPos) {

	if (shaderProgram == 0 || textureId == 0) {
		cerr << "Error Rendering for Planet: " << name << " due to missing texture data." << endl;
		return;
	}

	mat4 model = mat4(1.0f);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);

	model = translate(model, position);
	model = rotate(model, radians(axialTilt), vec3(1.0f, 0.0f, 0.0f));
	model = scale(model, vec3(radius));


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(view));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, value_ptr(cameraPos));

	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), 42.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "specularStrength"), 0.5f);
	glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), 0.1f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(glGetUniformLocation(shaderProgram, "diffuseMap"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMapId);
	glUniform1i(glGetUniformLocation(shaderProgram, "normalMap"), 1);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}