#include "window.h"
#include <stdexcept>

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowPtr->framebufferResized = true;
    windowPtr->width = width;
    windowPtr->height = height;
}

Window::Window(int w, int h, std::string name)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);// only for full screen

	window = glfwCreateWindow(mode->width, mode->height, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(window);
}

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}
