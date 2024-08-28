#pragma once
#include "window.h"
#include "../mesh/mesh.h"

class MainCamera
{
public:
    const float FOV = 45.0f;
    float mouseSensitivity = 0.005f;
    float movementSpeed = 0.005f;
    glm::vec3 velocity;
    glm::vec3 position;
    // vertical rotation
    float pitch{ 0.f };
    // horizontal rotation
    float yaw{ 0.f };
    
    MainCamera();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetRotationMatrix();

    void ProcessKeyboard(GLFWwindow* window, float deltaTime, GLboolean constrainPitch = true);
    void MouseUpdate(GLFWwindow* window, double inXpos, double inYpos);

    void Update();
private:
    double lastX = 0;
    double lastY = 0;
    int firstMouse = true;
};

