#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SimulationManager.h"
#include "Controls.h"

using namespace std;
using namespace glm;

SimulationManager simulator;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    Controls controls;

    GLFWwindow* window = glfwCreateWindow(
        controls.getWidth(), controls.getHeight(), "ASTRA Space", NULL, NULL
    );
    if (window == NULL) {
        cout << "Failed to Create Window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetWindowUserPointer(window, &controls);
    glfwSetFramebufferSizeCallback(window, Controls::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, Controls::mouse_callback);
    glfwSetMouseButtonCallback(window, Controls::mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to Initialize Glad" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vec3 lightPos(0.0f, 0.0f, 0.0f);

    vec3 sunPos(0.0f);
    vec3 sunVel(0.0f);
    simulator.addStar("SampleSun", 234.0f, 19890000000.0f, 120600.0f, 100000.0f, 6000.0f, sunPos, sunVel, 0.0f, "textures/sun.jpg");

    CelestialObject* sun = simulator.getCelestialObject("SampleSun");

    vec3 earthPos(39720.0f, 0.0f, 0.0f);
    vec3 earthVel = simulator.calculateOrbitalVelocity(earthPos, sun);
    simulator.addPlanet("Earth", 9.8f, 597200000.0f, 1.0f, 0.0f, earthPos, 63710.0f, earthVel, 0.0f, 0.0f, "textures/earth.jpg");

    CelestialObject* earth = simulator.getCelestialObject("Earth");

    vec3 venusPos(49720.0f, 0.0f, 0.0f);
    vec3 venusVel = simulator.calculateOrbitalVelocity(venusPos, earth);
    simulator.addPlanet("Venus", 4.0f, 4972.0f, 1.0f, 0.0f, venusPos, 60510.0f, venusVel, 0.0f, 0.0f, "textures/venus.jpg");

    float timeFactor = 1.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        controls.updateDeltaTime(currentFrame);
        controls.processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 projection = perspective(
            radians(controls.getFov()),
            (float)controls.getWidth() / (float)controls.getHeight(),
            0.1f, 700000.0f
        );
        mat4 view = lookAt(
            controls.getCameraPos(),
            controls.getCameraPos() + controls.getCameraFront(),
            controls.getCameraUp()
        );

        simulator.update(controls.getDeltaTime(), timeFactor);
        simulator.render(view, projection, lightPos, controls.getCameraPos(), timeFactor);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}