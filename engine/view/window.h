#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	int width;
	int height;
	bool framebufferResized = false;

	std::string windowName;
	GLFWwindow* window;
public:
	Window(int w, int h, std::string name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool ShouldClose() { return glfwWindowShouldClose(window); }
	VkExtent2D GetExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
	bool WasWindowResized() { return framebufferResized; }
	void ResetWindowResizedFlag() { framebufferResized = false; }
	GLFWwindow* GetGLFWwindow() const { return window; }

	void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
};
