#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Loader.h"

class Skybox {
public:
    Skybox(const char* texturePath);
    ~Skybox();

    void init();
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);

private:
    GLuint VAO, VBO, EBO;
    GLuint textureId;
    GLuint shaderProgram;
    int indexCount;
    Loader loader;

    void generateSphere(float radius, int sectors, int stacks);
};