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
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        out vec3 fragWorldPos;
        void main() {
            fragWorldPos = aPos;
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    const char* fShader = R"(
        #version 330 core
        in vec3 fragWorldPos;
        out vec4 FragColor;
        uniform float gridSize;

        void main() {
            float dist = length(fragWorldPos.xz);
        
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
void OrbitalGrid::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) {
    glUseProgram(shaderProgram);

    float step = size / divisions;

    float offsetX = fmod(cameraPos.x, step);
    float offsetZ = fmod(cameraPos.z, step);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-offsetX, -cameraPos.y, -offsetZ));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
}