#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PlanetManager.h"
#include "StarManager.h"

using namespace std;
using namespace glm;

unsigned int WIDTH = 1280;
unsigned int HEIGHT = 720;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

bool firstMouse = true, mousePressed = false;

float cameraYaw = -90.0f, cameraPitch = 0.0f, fov = 45.0f, cameraSpeed = 3000.0f;

vec3 cameraPos(0.0f, 0.0f, 80.0f);
vec3 cameraFront(0.0f, 0.0f, -10.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f, lastFrame = 0.0f;

StarManager starManager;
PlanetManager planetManager;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    float speed = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && fov < 55.0f) fov = fov + 0.01f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && fov > 1.0f) fov = fov - 0.01f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = xpos;
            lastY = ypos;
            firstMouse = true;  
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mousePressed) return;  

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    cameraYaw += xoffset;
    cameraPitch += yoffset;
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(front);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ASTRA Space", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to Create Window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to Initialize Glad" << endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vec3 lightPos(0.0f, 0.0f, 0.0f);

	float sunMass = 1989300.0f; // test random values
    
    planetManager.addPlanet("Mercury", 3.7f, 33000.0f, sunMass, 0.0f, 570000.0f, 4879.0f, 0.1f, 0.2f, 9.0f, "textures/mercury.jpg");
    planetManager.addPlanet("Venus", 8.9f, 4870000.0f, sunMass, 0.0f, 1080000.0f, 12104.0f, 0.1f, 0.2f, 6.0f, "textures/venus.jpg");
    planetManager.addPlanet("Earth", 9.8f, 5970000.0f, sunMass, 0.0f, 1490000.0f, 12756.0f, 0.1f, 0.4f, 0.0f, "textures/earth.jpg");
    planetManager.addPlanet("Mars", 3.7f, 642000.0f, sunMass, 0.0f, 2280000.0f, 6792.0f, 0.1f, 0.1f, 3.0f, "textures/mars.jpg");
    planetManager.addPlanet("Jupiter", 23.1f, 18980000.0f, sunMass, 0.0f, 7780000.0f, 142984.0f, 0.1f, 0.1f, 17.0f, "textures/jupiter.jpg");
    planetManager.addPlanet("Saturn", 23.1f, 5680000.0f, sunMass, 0.0f, 14320000.0f, 120536.0f, 0.1f, 0.1f, 2.0f, "textures/saturn.jpg");
    planetManager.addPlanet("Uranus", 23.1f, 8680000.0f, sunMass, 0.0f, 28670000.0f, 51118.0f, 0.1f, 0.1f, 20.0f, "textures/uranus.jpg");
    planetManager.addPlanet("Neptune", 23.1f, 1020000.0f, sunMass, 0.0f, 45150000.0f, 49528.0f, 0.1f, 0.1f, 30.0f, "textures/neptune.jpg");
    planetManager.addPlanet("Sun", 274.0f, 33000.0f, sunMass, 0.0f, 0.1f, 1284.0f, 0.0f, 0.0f, 0.0f, "textures/sun.jpg");

    vec3 sunPos(0.0f, 0.0f, 0.0f);
    starManager.addStar("Sun", 274.0f, 28980000.0f, 142984.0f, 100.0f, 5778.0f, sunPos, 0.1f, "textures/sun.jpg");

    float timeFactor = 0.01f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 projection = perspective(radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 700000.0f);
        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        planetManager.renderPlanets(view, projection, lightPos, cameraPos, currentFrame, timeFactor);
        starManager.renderStars(view, projection, lightPos, cameraPos, currentFrame, timeFactor);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}