// for non keyboard callback handling

#include "globalOpenGL.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

extern glm::vec3 g_cameraEye;

bool isShiftDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || 
        (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) )
    {
        return true;
    }
    return false;
}

bool isControlDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool isAltDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
    if ( isShiftDown(window) ) 
    {
        return false;
    }

    if ( isControlDown(window) ) 
    {
        return false;
    }

    if ( isAltDown(window) ) 
    {
        return false;
    }

    // Nothing is down
    return true;
}

void handleKeyboardAsync(GLFWwindow* window)
{
    const float CAMERA_MOVE_SPEED = 0.1f;

    if ( isShiftDown(window) )
    {
        // Keys move the target
    }

    if (areAllModifiersUp(window))
    {

        // Move the camera 
        // Or "if ( glfwGetKey(window, GLFW_KEY_A)"
        if ( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
        {
            // Go left
            ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            // Go right
            ::g_cameraEye.x += CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            // Go forward
            ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            // Go back
            ::g_cameraEye.z += CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            // Go down
            ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            // Go up
            ::g_cameraEye.y += CAMERA_MOVE_SPEED;
        }
    }//if (areAllModifiersUp(window)

    return;
}