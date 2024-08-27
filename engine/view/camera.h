#pragma once
#include "window.h"
#include "../mesh/mesh.h"
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), float yaw = YAW, float pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
    {
        Position = position;
        WorldUp = up;
        //Up = up;
        Yaw = yaw;
        Pitch = pitch;
        //Front = front;
        updateCameraVectors();
        //glfwSetCursorPosCallback(window.GetGLFWwindow(),(GLFWcursorposfun)mouse_callback);
        //glfwSetScrollCallback(window.GetGLFWwindow(), camera.scroll_callback);
    }

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    int printCount = 0;
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        auto center = Position + Front;

        if (printCount < 5) {
            std::cout << "Position: " << std::endl;
            for (int i = 0; i < Position.length(); i++) {
                std::cout << Position[i];
            }
            std::cout << std::endl;

            std::cout << "Front: " << std::endl;
            for (int i = 0; i < Front.length(); i++) {
                std::cout << Front[i];
            }
            std::cout << std::endl;

            std::cout << "Position + Front: " << std::endl;
            for (int i = 0; i < center.length(); i++) {
                std::cout << center[i];
            }
            std::cout << std::endl;

            std::cout << "Up: " << std::endl;
            for (int i = 0; i < Up.length(); i++) {
                std::cout << Up[i];
            }
            std::cout << std::endl;
            
            printCount++;
        }

        return glm::lookAt(Position, center, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessKeyboard(GLFWwindow* window, float deltaTime, GLboolean constrainPitch = true)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float cameraSpeed = static_cast<float>(2.5 * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Position += cameraSpeed * Front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Position -= cameraSpeed * Front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;

        GLdouble xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        mouse_callback(window, xPos, yPos);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            Pitch += 3;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            Pitch -= 3;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            Yaw += 3;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            Yaw -= 3;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            Fov += 5.0f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            Fov -= 5.0f;

        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 45.0f)
            Fov = 45.0f;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void mouse_callback(GLFWwindow* window, double inXpos, double inYpos)
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

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.z = sin(glm::radians(Pitch));
        Front = glm::normalize(front);
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Fov -= (float)yoffset;
        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 45.0f)
            Fov = 45.0f;
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Fov -= (float)yoffset;
        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 45.0f)
            Fov = 45.0f;
    }

private:
    double lastX = 0;
    double lastY = 0;
    int firstMouse = true;
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

/*
class Camera
{
public:
    struct KeyMappings {
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_E;
        int moveDown = GLFW_KEY_Q;
        int lookLeft = GLFW_KEY_LEFT;
        int lookRight = GLFW_KEY_RIGHT;
        int lookUp = GLFW_KEY_UP;
        int lookDown = GLFW_KEY_DOWN;
    };

    void MoveInPlaneXZ(GLFWwindow* window, float dt, UniformBufferObject& transform);

    KeyMappings keys{};
    float moveSpeed{ 3.f };
    float lookSpeed{ 1.5f };

    glm::vec3 velocity;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::normalize(position - target);
    glm::vec3 upWorldSpace = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(upWorldSpace, direction));// x-axis of camera space
    glm::vec3 up = glm::cross(direction, right);

    UniformBufferObject transform{};

    // vertical rotation
    float pitch{ 0.f };
    // horizontal rotation
    float yaw{ 0.f };

    glm::mat4 GetViewMatrix();
    glm::mat4 GetRotationMatrix();

    void Update(GLFWwindow* window, UniformBufferObject& transform);
};*/

