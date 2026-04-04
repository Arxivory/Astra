#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Loader.h"

class OrbitalGrid {
public:
    OrbitalGrid(float size, int divisions);
    ~OrbitalGrid();

    void init();
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO, VBO;
    GLuint shaderProgram;
    int vertexCount;
    float size;
    int divisions;

    void setupShaders();
};