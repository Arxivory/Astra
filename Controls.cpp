#include "Controls.h"

Controls::Controls() {}
Controls::~Controls() {}

// Static trampoline for framebuffer size
void Controls::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Controls* self = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (self) self->handleFramebufferSize(window, width, height);
}

// Static trampoline for mouse movement
void Controls::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Controls* self = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (self) self->handleMouse(window, xpos, ypos);
}

// Static trampoline for mouse button
void Controls::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Controls* self = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (self) self->handleMouseButton(window, button, action, mods);
}

// Instance method for framebuffer size
void Controls::handleFramebufferSize(GLFWwindow* window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

// Instance method for mouse movement
void Controls::handleMouse(GLFWwindow* window, double xpos, double ypos) {
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

// Instance method for mouse button
void Controls::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
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

void Controls::processInput(GLFWwindow* window) {
    if (followTarget) {
        vec3 desiredPos = followTarget->getPosition() + followOffset + followTarget->getRadius();
        cameraPos = glm::mix(cameraPos, desiredPos, lerpSpeed * deltaTime);
    }
    float speed = cameraSpeed * deltaTime;

    bool moving = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { cameraPos += speed * cameraFront; moving = true; }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { cameraPos -= speed * cameraFront; moving = true; }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed; moving = true; }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed; moving = true; }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && fov < 55.0f) fov = fov + 0.01f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && fov > 1.0f) fov = fov - 0.01f;

	if (moving) followTarget = nullptr;
}

void Controls::updateDeltaTime(float currentFrame) {
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

vec3 Controls::getMouseRayDirection(GLFWwindow* window) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float x = (2.0f * mouseX) / WIDTH - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / HEIGHT;
    float z = 1.0f;
    vec3 rayNDS = vec3(x, y, z);
    vec4 rayClip = vec4(rayNDS.x, rayNDS.y, -1.0f, 1.0f);
    mat4 projection = perspective(radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
    mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mat4 invProj = inverse(projection);
    mat4 invView = inverse(view);
    vec4 rayEye = invProj * rayClip;
    rayEye.z = -1.0f; rayEye.w = 0.0f;
    vec4 rayWorld = invView * rayEye;
    return normalize(vec3(rayWorld));
}
