#include "globalOpenGL.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "cMeshObject.h"

extern glm::vec3 g_cameraEye;//  = glm::vec3(0.0, 0.0, -20.0f);

// note that this is a pointer because 
extern std::vector< cMeshObject* > g_MeshesToDraw;


// The item that I'm currently selecting
unsigned int g_selectedObjectIndex = 0;


bool isShiftDown(int mods)
{
    if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Yup
        return true;
    }
    return false;
}
bool isCntlDown(int mods)
{
    if ( (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Yup
        return true;
    }
    return false;
}
bool isAltDown(int mods)
{
    if ( (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Yup
        return true;
    }
    return false;
}

bool areAnyModifiersDown(int mods)
{
    if (isShiftDown(mods)) {
        return true;
    }
    if (isAltDown(mods)) {
        return true;
    } 
    if (isCntlDown(mods)) {
        return true;
    }
    return false;
}
//     void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    const float CAMERA_SPEED = 0.1f;

    // if shift is down (just shift) then move the selected object
    if (mods == GLFW_MOD_SHIFT)       // 0001   // 0010  // 0100  // 0111
    {
        // ONLY the shift key (either one) is down


    }//if ( mods == GLFW_MOD_SHIFT )

    // If one of the keys is shift
    //  Shift -- 0001
    //  Cont  -- 0010
    //  Alt   -- 0100
    // Press all 3 --> 0111
    //     & 0001  --- 0001 // "bitwise and is a 'mask' operation
    //             --------
    //                 0001

    const float OBJECT_MOVE_SPEED = 0.5f;

    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Then shift (and maybe something else) is down
        
        if (key == GLFW_KEY_A)  
        {  
            ::g_MeshesToDraw[ ::g_selectedObjectIndex ]->position.x -= OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_D)  {
            ::g_MeshesToDraw[::g_selectedObjectIndex]->position.x += OBJECT_MOVE_SPEED;
        }

        if (key == GLFW_KEY_Q)  
        {  
            ::g_MeshesToDraw[ ::g_selectedObjectIndex ]->position.y -= OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_E)  {
            ::g_MeshesToDraw[::g_selectedObjectIndex]->position.y += OBJECT_MOVE_SPEED;
        }

        if (key == GLFW_KEY_W)  
        {  
            ::g_MeshesToDraw[ ::g_selectedObjectIndex ]->position.z += OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_S)  {
            ::g_MeshesToDraw[::g_selectedObjectIndex]->position.z -= OBJECT_MOVE_SPEED;
        }

        if (key == GLFW_KEY_O )
        {
            // TODO: ERROR here if out of range
            ::g_selectedObjectIndex--;
        }
        if (key == GLFW_KEY_P )
        {
            ::g_selectedObjectIndex++;
        }

        if ( key == GLFW_KEY_ENTER )
        {
            // theFile << ::g_MeshesToDraw.size() << " "
            // 
            // Save the "scene"
            for ( cMeshObject* pCM : ::g_MeshesToDraw )
            {
                // Baft the object properties to a file
                // theFile << pCM->meshName << " "
                // theFile << pCM->position.x
                // theFile << pCM->position.y
                // .. and so on
            }

        }
        if ( key == GLFW_KEY_L )
        {
            // Load the scene
            // Clear the current g_MeshesToDraw
            // Loop through and load the properties from the file
        }

    //    if (key == GLFW_KEY_W){
    //        ::g_cameraEye.z -= CAMERA_SPEED;
    //    }
    //    if (key == GLFW_KEY_S)
    //    {
    //        // Go back
    //        ::g_cameraEye.z += CAMERA_SPEED;
    ////        ::g_cameraTarget.x += CAMERA_SPEED;
    //    }

    //    if (key == GLFW_KEY_Q)
    //    {
    //        // Go down
    //        ::g_cameraEye.y -= CAMERA_SPEED;
    ////        ::g_cameraTarget.x -= CAMERA_SPEED;
    //    }
    //    if (key == GLFW_KEY_E)
    //    {
    //        // Go up
    //        ::g_cameraEye.y += CAMERA_SPEED;
    ////        ::g_cameraTarget.x += CAMERA_SPEED;
    //    }


    }//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)




    // Are all modifiers up?
    if ( ! areAnyModifiersDown(mods) )
    {

        // Move the camera 
        if (key == GLFW_KEY_A)
        {
            // Go left
            ::g_cameraEye.x -= CAMERA_SPEED;
    //        ::g_cameraTarget.x -= CAMERA_SPEED;
        }
        if (key == GLFW_KEY_D)
        {
            // Go right
            ::g_cameraEye.x += CAMERA_SPEED;
    //        ::g_cameraTarget.x += CAMERA_SPEED;
        }

        if (key == GLFW_KEY_W)
        {
            // Go forward
            ::g_cameraEye.z -= CAMERA_SPEED;
    //        ::g_cameraTarget.x -= CAMERA_SPEED;
        }
        if (key == GLFW_KEY_S)
        {
            // Go back
            ::g_cameraEye.z += CAMERA_SPEED;
    //        ::g_cameraTarget.x += CAMERA_SPEED;
        }

        if (key == GLFW_KEY_Q)
        {
            // Go down
            ::g_cameraEye.y -= CAMERA_SPEED;
    //        ::g_cameraTarget.x -= CAMERA_SPEED;
        }
        if (key == GLFW_KEY_E)
        {
            // Go up
            ::g_cameraEye.y += CAMERA_SPEED;
    //        ::g_cameraTarget.x += CAMERA_SPEED;
        }
    }// if ( ! areAnyModifiersDown(mods) )
    return;
}
