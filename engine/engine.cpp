#include "engine.h"

// TODO REFACOR IN PROGRESS . . .

Engine::Engine() {

}

Engine::~Engine()
{
}

void Engine::MainLoop()
{
    while (!glfwWindowShouldClose(window.GetGLFWwindow())) {
        glfwPollEvents();
        vulkanPipline.DrawFrame();
    }

    vkDeviceWaitIdle(device.GetDevice());
}
