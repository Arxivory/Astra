#include "Planet.h"

using namespace std;


Planet::Planet(string name, float gravity, float mass, float centralMass, float axialTilt, float distance, float size,
	float orbitSpeed, float rotationSpeed, float initialAngle) : CelestialObject(name, mass, size, vec3(distance, 0.0f, 0.0f), vec3(0.0f)),
	name(name),
	gravity(gravity),
	mass(mass* SCALE_FACTOR),
	centralMass(centralMass * SCALE_FACTOR),
	axialTilt(axialTilt),
	distance(distance * SCALE_FACTOR),
	size(size * SCALE_FACTOR),
	orbitSpeed(orbitSpeed),
	rotationSpeed(rotationSpeed),
	initialAngle(initialAngle),
	VAO(0), VBO(0), EBO(0), textureId(0) { }

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
}

void Planet::generateSphereMesh() {
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

void Planet::setup(const char *texturePath) {

	textureId = loader.loadTexture(texturePath);
	if (textureId != 0)
		cout << "Planet " << name << " Loaded Successfully" << endl;

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

	setupPosition();
}

void Planet::setupPosition() {
	position = vec3(distance, 0.0f, 0.0f);
	
	vec3 direction = position - vec3(0.0f, 0.0f, 0.0f); 

	float distanceCopy = length(direction);

	float orbitalSpeed = sqrt((G * centralMass) / distanceCopy);
	vec3 orbitDir = normalize(cross(vec3(0.0f, 1.0f, 0.0f), direction));
	velocity = orbitDir * orbitalSpeed;
}

void Planet::updatePhysics(float deltaTime) {
	vec3 acceleration = force / mass;

	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;
}

void Planet::render(float currentFrame, float timeFactor, const mat4& view,
	const mat4& projection, const vec3& lightPos, const vec3& cameraPos) {

	mat4 model = mat4(1.0f);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);

	model = translate(model, position);
	model = rotate(model, radians(axialTilt), vec3(1.0f, 0.0f, 0.0f));
	model = scale(model, vec3(size));


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(view));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, value_ptr(cameraPos));

	glBindTexture(GL_TEXTURE_2D, textureId);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}
