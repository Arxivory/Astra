#pragma once
#ifndef CONTROLS_H
#define CONTROLS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;
using namespace glm;

class Controls {
public:
    Controls();
    ~Controls();

    // Static trampoline callbacks for GLFW
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    // Instance methods
    void processInput(GLFWwindow* window);
    void updateDeltaTime(float currentFrame);

    // Getters for camera state
    const vec3& getCameraPos() const { return cameraPos; }
    const vec3& getCameraFront() const { return cameraFront; }
    const vec3& getCameraUp() const { return cameraUp; }
    float getFov() const { return fov; }
    unsigned int getWidth() const { return WIDTH; }
    unsigned int getHeight() const { return HEIGHT; }
	float getDeltaTime() const { return deltaTime; }

private:
    // Instance methods called by static trampolines
    void handleFramebufferSize(GLFWwindow* window, int width, int height);
    void handleMouse(GLFWwindow* window, double xpos, double ypos);
    void handleMouseButton(GLFWwindow* window, int button, int action, int mods);

    // Camera and input state
    bool firstMouse = true, mousePressed = false;
    float cameraYaw = -90.0f, cameraPitch = 0.0f, fov = 45.0f, cameraSpeed = 3000.0f;
    vec3 cameraPos = vec3(0.0f, 0.0f, 80.0f);
    vec3 cameraFront = vec3(0.0f, 0.0f, -10.0f);
    vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
    float deltaTime = 0.0f, lastFrame = 0.0f;
    unsigned int WIDTH = 1280;
    unsigned int HEIGHT = 720;
    float lastX = WIDTH / 2.0f;
    float lastY = HEIGHT / 2.0f;
};

#endif // !CONTROLS_H
