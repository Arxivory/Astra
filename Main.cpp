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

//Reminders:
/*
	1. Scale distances and sizes appropriately to ensure visibility and performance.
	2. Remove unecessary parameters from CelestialObject and derived classes.
	3. Separate hardcoded shader code into external files (Trajectory).
	4. Allow ringed planets functionality (Saturn).
	5. Implement light emission for stars.
	6. Optimize physics calculations for larger simulations.
	7. Fix z fighting on planet surfaces.
	8. Logarithmic depth buffer for vast distances.
*/

void renderSelectedObjectInfo(CelestialObject* selected) {
    ImGui::Begin("Object Telemetry");
    if (selected) {
        ImGui::Text("Name: %s", selected->getName().c_str());
        ImGui::Separator();
        ImGui::Text("Position: %.2f, %.2f, %.2f", selected->getPosition().x, selected->getPosition().y, selected->getPosition().z);
        ImGui::Text("Velocity: %.2f km/s", length(selected->getVelocity()));

        if (ImGui::Button("Focus Camera")) {
            // Implement a camera 'lock' here later!
        }
    }
    else {
        ImGui::Text("No object selected. Click a celestial object to view data.");
    }
    ImGui::End();
}

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
    simulator.addStar("Sun", 234.0f, 1989999988.0f, 120600.0f, 100000.0f, 6000.0f, sunPos, sunVel, 0.0f, "textures/sun.jpg");

    CelestialObject* sun = simulator.getCelestialObject("Sun");

	vec3 mercuryPos = vec3(38700.0f, 0.0f, 0.0f);
	vec3 mercuryVel = simulator.calculateOrbitalVelocity(mercuryPos, sun);
	simulator.addPlanet("Mercury", 0.165f, 53000000.0f, sun->getMass(), 0.034f, mercuryPos, 4879.0f / 2.0f, mercuryVel, 10.83f, 0.0f, "textures/mercury.jpg", "textures/mercury_normal.jpg");

	vec3 venusPos = vec3(72300.0f, 0.0f, 0.0f);
	vec3 venusVel = simulator.calculateOrbitalVelocity(venusPos, sun);
	simulator.addPlanet("Venus", 0.185f, 48700000.0f, sun->getMass(), 177.4f, venusPos, 12104.0f / 2.0f, venusVel, -6.52f, 0.0f, "textures/venus.jpg", "textures/mercury_normal.jpg");

	vec3 earthPos = vec3(100000.0f, 0.0f, 0.0f);
	vec3 earthVel = simulator.calculateOrbitalVelocity(earthPos, sun);
	simulator.addPlanet("Earth", 1.0f, 39700000.0f, sun->getMass(), 23.44f, earthPos, 12742.0f / 2.0f, earthVel, 15.0f, 0.0f, "textures/earth.jpg", "textures/earth_normal.jpg");

	CelestialObject* earth = simulator.getCelestialObject("Earth");
	vec3 moonPos = earth->getPosition() + vec3(0.0f, 0.0f, 0.6244f * 1000.0f);
	vec3 moonVel = simulator.calculateOrbitalVelocity(moonPos, earth) + earth->getVelocity();
	simulator.addPlanet("Moon", 0.165f, 9395000.0f, earth->getMass(), 6.68f, moonPos, 2974.8f / 2.0f, moonVel, 10.0f, 0.0f, "textures/moon.jpg", "textures/mercury_normal.jpg");

	vec3 marsPos = vec3(152400.0f, 0.0f, 0.0f);
	vec3 marsVel = simulator.calculateOrbitalVelocity(marsPos, sun);
	simulator.addPlanet("Mars", 0.107f, 64200000.0f, sun->getMass(), 25.19f, marsPos, 6779.0f / 2.0f, marsVel, 14.62f, 0.0f, "textures/mars.jpg", "textures/mercury_normal.jpg");

	vec3 jupiterPos = vec3(520300.0f, 0.0f, 0.0f);
	vec3 jupiterVel = simulator.calculateOrbitalVelocity(jupiterPos, sun);
	simulator.addPlanet("Jupiter", 2.528f, 1898000000.0f, sun->getMass(), 3.13f, jupiterPos, 139820.0f / 2.0f, jupiterVel, 12.6f, 0.0f, "textures/jupiter.jpg", "textures/mercury_normal.jpg");
	
    float timeFactor = 5.0f;

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

        static bool wasMouseDown = false;
        bool isMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        if (isMouseDown && !wasMouseDown && !ImGui::GetIO().WantCaptureMouse) {
            vec3 rayDir = controls.getMouseRayDirection(window);

            CelestialObject* selected = simulator.findSelectedObject(controls.getCameraPos(), rayDir);

            if (selected) {
                simulator.setSelectedObject(selected);
                cout << "Selected: " << selected->getName() << endl;
            }
        }
        wasMouseDown = isMouseDown;

        simulator.update(controls.getDeltaTime(), timeFactor);
        simulator.render(view, projection, lightPos, controls.getCameraPos(), timeFactor);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Simulation Controls");
        ImGui::SliderFloat("Time Factor", &timeFactor, 0.00000001f, 500.0f, "%.6f");
        ImGui::SliderFloat("Camera Speed", &controls.cameraSpeed, 1.0f, 10000.0f, "%.1f");
		ImGui::Button("Toggle Trajectory");
		if (ImGui::IsItemClicked()) {
			simulator.toggleTrajectory();
		}
        ImGui::End();

		renderSelectedObjectInfo(simulator.getSelectedObject());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}