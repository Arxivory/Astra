#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include "CelestialObject.h"

class GizmoManager {
public:

    void update(CelestialObject* selected, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) {
        if (!selected) return;

        ImGuizmo::BeginFrame();
        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

        // 1. Calculate Relative Position for the Gizmo
        glm::vec3 relativePos = selected->getPosition() - cameraPos;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), relativePos);

        // 2. Manipulate using the relative model matrix
        if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
            ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(model))) {

            float matrixTranslation[3], matrixRotation[3], matrixScale[3];
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), matrixTranslation, matrixRotation, matrixScale);

            // 3. Convert back to World Space by adding cameraPos
            glm::vec3 newRelativePos(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
			glm::vec3 newWorldPos = newRelativePos + cameraPos;
            selected->setPosition(newWorldPos);

            glm::vec3 zeroVelocity(0.0f);
            selected->setVelocity(zeroVelocity);
        }
    }

    bool isUsing() const { return ImGuizmo::IsUsing(); }
};