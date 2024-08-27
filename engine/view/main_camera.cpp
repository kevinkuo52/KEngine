#include "main_camera.h"

MainCamera::MainCamera()
{
    velocity = glm::vec3(0.f);
    position = glm::vec3(0, 0, 5);
    pitch = 0;
    yaw = 0;
}

glm::mat4 MainCamera::GetViewMatrix()
{
    // to create a correct model view, we need to move the world in opposite
    // direction to the camera
    //  so we will create the camera model matrix and invert
    glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.f), position);
    glm::mat4 cameraRotation = GetRotationMatrix();
    return glm::inverse(cameraTranslation * cameraRotation);
}

glm::mat4 MainCamera::GetRotationMatrix()
{
    // fairly typical FPS style camera. we join the pitch and yaw rotations into
    // the final rotation matrix

    glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3{ 1.f, 0.f, 0.f });
    glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3{ 0.f, -1.f, 0.f });

    return glm::mat4_cast(yawRotation) * glm::mat4_cast(pitchRotation);
}

void MainCamera::ProcessKeyboard(GLFWwindow* window, float deltaTime, GLboolean constrainPitch)
{
    velocity.x = 0;
    velocity.y = 0;
    velocity.z = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
        velocity.z = -1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
        velocity.z = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        velocity.x = -1;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
    {
        velocity.x = 1;
    }

    GLdouble xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    MouseUpdate(window, xPos, yPos);
}

void MainCamera::MouseUpdate(GLFWwindow* window, double inXpos, double inYpos)
{
    float xpos = static_cast<float>(inXpos);
    float ypos = static_cast<float>(inYpos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;
}

void MainCamera::Update()
{
	glm::mat4 cameraRotation = GetRotationMatrix();
	position += glm::vec3(cameraRotation * glm::vec4(velocity * movementSpeed, 0.f));
}
