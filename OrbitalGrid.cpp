#include "OrbitalGrid.h"

OrbitalGrid::OrbitalGrid(float size, int divisions)
    : size(size), divisions(divisions), VAO(0), VBO(0), shaderProgram(0) {
}

OrbitalGrid::~OrbitalGrid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void OrbitalGrid::setupShaders() {
    const char* vShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * vec4(aPos, 1.0);
        }
    )";

    const char* fShader = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.3, 0.3, 0.3, 0.4);
        }
    )";

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShader, NULL);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShader, NULL);
    glCompileShader(fragment);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
}

void OrbitalGrid::init() {
    setupShaders();

    std::vector<float> vertices;
    float step = size / divisions;

    for (int i = 0; i <= divisions; ++i) {
        float pos = -size / 2.0f + i * step;

        vertices.push_back(pos); vertices.push_back(0.0f); vertices.push_back(-size / 2.0f);
        vertices.push_back(pos); vertices.push_back(0.0f); vertices.push_back(size / 2.0f);

        vertices.push_back(-size / 2.0f); vertices.push_back(0.0f); vertices.push_back(pos);
        vertices.push_back(size / 2.0f);  vertices.push_back(0.0f); vertices.push_back(pos);
    }

    vertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void OrbitalGrid::render(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
}