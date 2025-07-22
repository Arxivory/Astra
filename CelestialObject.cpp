#include "CelestialObject.h"

CelestialObject::CelestialObject(string name, float mass, float radius, vec3 &initialPosition, 
	vec3 &initialVelocity) :
	name(name),
	mass(mass),
	radius(radius),
	position(initialPosition),
	velocity(initialVelocity),
	force(0.0f),
	maxTrajectoryPoints(INT16_MAX) { }

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

void CelestialObject::setupTrajectoryShaders() {
	const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * vec4(aPos, 1.0);
        }
    )";

	const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 lineColor;
        void main() {
            FragColor = vec4(lineColor, 1.0);
        }
    )";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	trajectoryShaderProgram = glCreateProgram();
	glAttachShader(trajectoryShaderProgram, vertexShader);
	glAttachShader(trajectoryShaderProgram, fragmentShader);
	glLinkProgram(trajectoryShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void CelestialObject::setupTrajectoryBuffers() {
	glGenVertexArrays(1, &trajectoryVAO);
	glGenBuffers(1, &trajectoryVBO);

	glBindVertexArray(trajectoryVAO);
	glBindBuffer(GL_ARRAY_BUFFER, trajectoryVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void CelestialObject::updateTrajectory() {
	trajectoryPoints.push_back(position);

	if (trajectoryPoints.size() > maxTrajectoryPoints) {
		trajectoryPoints.erase(trajectoryPoints.begin());
	}
}

void CelestialObject::renderTrajectory(const mat4& view, const mat4& projection) {
	if (trajectoryPoints.size() < 2) return;

	glUseProgram(trajectoryShaderProgram);
	glBindVertexArray(trajectoryVAO);

	glBindBuffer(GL_ARRAY_BUFFER, trajectoryVBO);
	glBufferData(GL_ARRAY_BUFFER, trajectoryPoints.size() * 3 * sizeof(float),
		trajectoryPoints.data(), GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(glGetUniformLocation(trajectoryShaderProgram, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(trajectoryShaderProgram, "projection"), 1, GL_FALSE, value_ptr(projection));

	vec3 lineColor = vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(trajectoryShaderProgram, "lineColor"), 1, value_ptr(lineColor));

	glDrawArrays(GL_LINE_STRIP, 0, trajectoryPoints.size());
}