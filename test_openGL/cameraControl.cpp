#include "cameraControl.h"

#include <algorithm>

// Camera parameters
float camX = 2.0f;
float camY = 2.0f;
float camZ = 2.0f;
float camSpeed = 0.1f;


// Process scroll input
void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Adjust the camera speed based on the vertical scroll (yoffset)
    const float scrollSpeed = 0.1f;
    float newSpeed = scrollSpeed * static_cast<float>(ypos);

    // Limit the minimum speed to prevent too slow movement
    const float minSpeed = 0.1f;
    float updatedSpeed = std::max(camSpeed + newSpeed, minSpeed);

    // Update the global camera speed
    camSpeed = updatedSpeed;
}

// WASD and QE command input
void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.1f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camZ -= cameraSpeed;

    // Move the camera backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camZ += cameraSpeed;

    // Move the camera right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camX += cameraSpeed;

    // Move the camera left 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camX -= cameraSpeed;

    // Move the camera upward
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camY -= cameraSpeed;

    // Move the camera downward
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camY += cameraSpeed;

}
// Mouse control input
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Static variables to keep track of the last mouse position and first mouse movement
    static float lastX = 400.0f;
    static float lastY = 300.0f;
    static bool firstMouse = true;

    // Check if it's the first mouse movement
    if (firstMouse)
    {
        // Set the last mouse position to the current position
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    // Calculate the offset of mouse movement
    float xOffset = static_cast<float>(xpos) - lastX;
    float yOffset = lastY - static_cast<float>(ypos);  // Reversed because y-coordinates range from bottom to top

    // Sensitivity factor to control the rotation speed based on mouse movement
    const float sensitivity = 0.05f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // Update the camera's orientation based on mouse movement
    camX += xOffset;
    camY += yOffset;

    // Limit the pitch to avoid camera flipping
    if (camY > 89.0f)
        camY = 89.0f;
    if (camY < -89.0f)
        camY = -89.0f;

    // Update the last mouse position
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
}

