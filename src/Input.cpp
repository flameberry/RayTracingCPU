#include "Input.h"

#include "Application.h"

namespace Flameberry {
    bool Input::IsKey(uint16_t key, uint16_t action)
    {
        GLFWwindow* window = Application::GetApp().GetMainWindow().GetGLFWwindow();
        return glfwGetKey(window, key) == action;
    }

    bool Input::IsMouseButton(uint16_t button, uint16_t action)
    {
        GLFWwindow* window = Application::GetApp().GetMainWindow().GetGLFWwindow();
        return glfwGetMouseButton(window, button) == action;
    }

    glm::vec2 Input::GetCursorPosition()
    {
        GLFWwindow* window = Application::GetApp().GetMainWindow().GetGLFWwindow();
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return glm::vec2((float)x, (float)y);
    }

    void Input::SetCursorMode(uint32_t cursorMode)
    {
        GLFWwindow* window = Application::GetApp().GetMainWindow().GetGLFWwindow();
        glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
    }
}