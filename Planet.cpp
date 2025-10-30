#include "Planet.h"

using namespace std;


Planet::Planet(string name, float gravity, float mass, float centralMass, float axialTilt, vec3& initialPosition, float size,
	vec3& initialVelocity, float rotationSpeed, float initialAngle) : CelestialObject(name, mass, size * SCALE_FACTOR, initialPosition, initialVelocity),
	gravity(gravity),
	mass(mass* SCALE_FACTOR),
	centralMass(centralMass * SCALE_FACTOR),
	axialTilt(axialTilt),
	distance(distance * SCALE_FACTOR),
	size(size * SCALE_FACTOR),
	orbitSpeed(orbitSpeed),
	rotationSpeed(rotationSpeed),
	initialAngle(initialAngle),
	VAO(0), VBO(0), EBO(0), textureId(0)  { }


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
	
			vec3 normal = normalize(vec3(x, z, y));

	        vertices.push_back(x);
			vertices.push_back(z);
			vertices.push_back(y);

			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);

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

	vector<vec3> tangents(vertices.size() / 8, vec3(0.0f));

	for (size_t i = 0; i < indices.size(); i += 3) {
		unsigned int i0 = indices[i];
		unsigned int i1 = indices[i + 1];
		unsigned int i2 = indices[i + 2];

		vec3 pos1(vertices[i0 * 8 + 0], vertices[i0 * 8 + 1], vertices[i0 * 8 + 2]);
		vec3 pos2(vertices[i1 * 8 + 0], vertices[i1 * 8 + 1], vertices[i1 * 8 + 2]);
		vec3 pos3(vertices[i2 * 8 + 0], vertices[i2 * 8 + 1], vertices[i2 * 8 + 2]);

		vec2 uv1(vertices[i0 * 8 + 6], vertices[i0 * 8 + 7]);
		vec2 uv2(vertices[i1 * 8 + 6], vertices[i1 * 8 + 7]);
		vec2 uv3(vertices[i2 * 8 + 6], vertices[i2 * 8 + 7]);

		vec3 edge1 = pos2 - pos1;
		vec3 edge2 = pos3 - pos1;
		vec2 deltaUV1 = uv2 - uv1;
		vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		vec3 tangent = f * (edge1 * deltaUV2.y - edge2 * deltaUV1.y);

		tangents[i0] += tangent;
		tangents[i1] += tangent;
		tangents[i2] += tangent;
	}

	for (auto& t : tangents)
		t = normalize(t);

	vector<float> finalVertices;
	for (size_t i = 0; i < vertices.size() / 8; ++i) {
		finalVertices.push_back(vertices[i * 8 + 0]);
		finalVertices.push_back(vertices[i * 8 + 1]);
		finalVertices.push_back(vertices[i * 8 + 2]);

		finalVertices.push_back(vertices[i * 8 + 3]); 
		finalVertices.push_back(vertices[i * 8 + 4]);
		finalVertices.push_back(vertices[i * 8 + 5]);

		finalVertices.push_back(vertices[i * 8 + 6]); 
		finalVertices.push_back(vertices[i * 8 + 7]);

		finalVertices.push_back(tangents[i].x);
		finalVertices.push_back(tangents[i].y);
		finalVertices.push_back(tangents[i].z);
	}
	vertices = finalVertices;


}

void Planet::setup(const char* texturePath, const char* normalPath) {

	textureId = loader.loadTexture(texturePath, true);
	normalMapId = loader.loadTexture(normalPath, false);

	if (textureId == 0 || normalMapId == 0) {
		cerr << "Error loading texture(s) for Planet: " << name << endl;
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);


}



void Planet::render(float currentFrame, float timeFactor, const mat4& view,
	const mat4& projection, const vec3& lightPos, const vec3& cameraPos) {

	if (shaderProgram == 0 || textureId == 0 || normalMapId == 0) {
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(glGetUniformLocation(shaderProgram, "diffuseMap"), 0);

	glUniform1f(glGetUniformLocation(shaderProgram, "bumpStrength"), 1.5f); 


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMapId);
	glUniform1i(glGetUniformLocation(shaderProgram, "normalMap"), 1);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
