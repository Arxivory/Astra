#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SimulationManager.h"
#include "Controls.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Constants.h"

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    vec3 lightPos(0.0f, 0.0f, 0.0f);

    vec3 sunPos(0.0f);
    vec3 sunVel(0.0f);
    simulator.addStar("Sun", 234.0f, 19890000000.0f, 120600.0f, 100000.0f, 6000.0f, sunPos, sunVel, 0.0f, "textures/sun.jpg");

    CelestialObject* sun = simulator.getCelestialObject("Sun");
	vec3 mercuryPos = vec3(38700.0f, 0.0f, 0.0f);
	vec3 mercuryVel = simulator.calculateOrbitalVelocity(mercuryPos, sun);
	simulator.addPlanet("Mercury", 0.165f, 0.330f, sun->getMass(), 0.034f, mercuryPos, 4879.0f / 2.0f, mercuryVel, 10.83f, 0.0f, "textures/mercury.jpg");

	CelestialObject* mercury = simulator.getCelestialObject("Mercury");
	vec3 venusPos = vec3(72300.0f, 0.0f, 0.0f);
	vec3 venusVel = simulator.calculateOrbitalVelocity(venusPos, sun);
	simulator.addPlanet("Venus", 0.185f, 4.87f, sun->getMass(), 177.4f, venusPos, 12104.0f / 2.0f, venusVel, -6.52f, 0.0f, "textures/venus.jpg");

	CelestialObject* venus = simulator.getCelestialObject("Venus");
	vec3 earthPos = vec3(100000.0f, 0.0f, 0.0f);
	vec3 earthVel = simulator.calculateOrbitalVelocity(earthPos, sun);
	simulator.addPlanet("Earth", 1.0f, 5.97f, sun->getMass(), 23.44f, earthPos, 12742.0f / 2.0f, earthVel, 15.0f, 0.0f, "textures/earth.jpg");

	CelestialObject* earth = simulator.getCelestialObject("Earth");
	vec3 marsPos = vec3(152400.0f * AU, 0.0f, 0.0f);
	vec3 marsVel = simulator.calculateOrbitalVelocity(marsPos, sun);
	simulator.addPlanet("Mars", 0.107f, 0.642f, sun->getMass(), 25.19f, marsPos, 6779.0f / 2.0f, marsVel, 14.62f, 0.0f, "textures/mars.jpg");

	CelestialObject* mars = simulator.getCelestialObject("Mars");
	vec3 jupiterPos = vec3(5.203f * AU, 0.0f, 0.0f);
	vec3 jupiterVel = simulator.calculateOrbitalVelocity(jupiterPos, sun);
	simulator.addPlanet("Jupiter", 2.528f, 1898.0f, sun->getMass(), 3.13f, jupiterPos, 139820.0f / 2.0f, jupiterVel, 12.6f, 0.0f, "textures/jupiter.jpg");
	
    float timeFactor = 0.01f;

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

        // --- ImGui controls --- for simulation only (not final)
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Simulation Controls");
        ImGui::SliderFloat("Time Factor", &timeFactor, 0.00000001f, 1.0f, "%.6f");
        ImGui::SliderFloat("Camera Speed", &controls.cameraSpeed, 1.0f, 4000.0f, "%.1f");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // --- End ImGui controls ---

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ImGui shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}