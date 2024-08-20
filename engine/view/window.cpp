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
    window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
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
