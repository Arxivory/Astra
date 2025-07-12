#include "Star.h"

Star::Star(string name, float gravity, float mass, float size, float luminosity, float temperature,
	vec3& position, float rotationSpeed) :
	name(name),
	gravity(gravity),
	mass(mass),
	size(size * SCALE_FACTOR),
	luminosity(luminosity),
	temperature(temperature),
	position(position),
	rotationSpeed(rotationSpeed),
	VAO(0), VBO(0), EBO(0), textureId(0) { }

void Star::setupShaderProgram() {
	shaderProgram = loader.createShaderProgram("shaders/vertex/StarVertexShader.glsl", "shaders/fragment/StarFragmentShader.glsl");

	if (shaderProgram == 0) {
		cerr << "Shader Program Failed to Create" << endl;
	}
	cout << "Star Shader Program created successfully for: " << name << endl;
}

void Star::init() {
	setupShaderProgram();
	generateSphereMesh();
}

void Star::generateSphereMesh() {
	int sectorCount = 64, stackCount = sectorCount;

	const float PI = 3.14159265359f;
	vertices.clear();
	indices.clear();

	float x, y, z, xy;
	float lengthInv = 1.0f;
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


			vertices.push_back(x);
			vertices.push_back(z);
			vertices.push_back(y);
			vertices.push_back(s);
			vertices.push_back(t);
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

void Star::setup(const char* texturePath) {

	textureId = loader.loadTexture(texturePath);
	if (textureId != 0)
		cout << "Star " << name << " Loaded Successfully" << endl;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void Star::render(float currentFrame, float timeFactor, const mat4& view,
	const mat4& projection, const vec3& lightPos, const vec3& cameraPos) {

	mat4 model = mat4(1.0f);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);

	float angularVelocity = 2.97e-6f; 

	float angle = angularVelocity * currentFrame * timeFactor;
	model = translate(model, position);
	model = rotate(model, angle, vec3(0.0f, 1.0f, 0.0f));
	model = scale(model, vec3(size));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(view));
	glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, value_ptr(cameraPos));

	glUniform1f(glGetUniformLocation(shaderProgram, "luminosity"), luminosity);
	glUniform1f(glGetUniformLocation(shaderProgram, "temperature"), temperature);
	glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
	glUniform1i(glGetUniformLocation(shaderProgram, "starTexture"), 0);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}
