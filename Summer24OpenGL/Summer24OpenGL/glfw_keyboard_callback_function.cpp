#include "globalOpenGL.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "cMeshObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "cLightManager.h"


//extern glm::vec3 g_cameraEye;//  = glm::vec3(0.0, 0.0, -20.0f);
//
//// note that this is a pointer because 
//extern std::vector< cMeshObject* > g_MeshesToDraw;
//
//extern cMeshObject* g_pSmoothSphere;
//
//extern cLightManager* g_pLights;// = NULL;

#include "sharedThings.h"

bool g_ShowLightDebugSphereThings = true;



// The item that I'm currently selecting
unsigned int g_selectedObjectIndex = 0;

unsigned int g_selectedLightIndex = 0;


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


    //// BYPASS THE REST
    //return;

    const float OBJECT_MOVE_SPEED = 0.5f;
    const float LIGHT_MOVE_SPEED = 0.5f;
    const float CAMERA_MOVE_SPEED = 1.0f;

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

    // ONLY the control key is down
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        if (key == GLFW_KEY_A)
        {
            ::g_pLights->theLights[::g_selectedLightIndex].position.x -= LIGHT_MOVE_SPEED;
//            ::g_pSmoothSphere->position.x -= OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_D) {
            ::g_pLights->theLights[::g_selectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }

        if (key == GLFW_KEY_Q)
        {
            ::g_pLights->theLights[::g_selectedLightIndex].position.y -= LIGHT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_E) {
            ::g_pLights->theLights[::g_selectedLightIndex].position.y += LIGHT_MOVE_SPEED;
        }

        if (key == GLFW_KEY_W)
        {
            ::g_pLights->theLights[::g_selectedLightIndex].position.z += LIGHT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_S) {
            ::g_pLights->theLights[::g_selectedLightIndex].position.z -= LIGHT_MOVE_SPEED;
        }

        // 1 and 2 changes linear attenuation
        // 	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
        if (key == GLFW_KEY_1)
        {
            // Drop by 1%
            ::g_pLights->theLights[::g_selectedLightIndex].atten.y *= 0.99f;
        }
        if (key == GLFW_KEY_2) {
            // Increase linear attenuation by 1%
            ::g_pLights->theLights[::g_selectedLightIndex].atten.y *= 1.01f;
        }

        // 3 and 4 changes quadratic
        // 	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
        if (key == GLFW_KEY_3)
        {
            // Drop by 1%
            ::g_pLights->theLights[::g_selectedLightIndex].atten.z *= 0.99f;
        }
        if (key == GLFW_KEY_4) {
            // Increase linear attenuation by 1%
            ::g_pLights->theLights[::g_selectedLightIndex].atten.z *= 1.01f;
        }

        // Inner angle
        if (key == GLFW_KEY_5) {
            // Make inner spot light angle less by 0.5%
            ::g_pLights->theLights[::g_selectedLightIndex].param1.y -= 0.05f;
        }
        if (key == GLFW_KEY_6) {
            ::g_pLights->theLights[::g_selectedLightIndex].param1.y += 0.05f;
        }
        // Outer angle
        if (key == GLFW_KEY_7) {
            // Make outer spot light angle less by 0.5%
            ::g_pLights->theLights[::g_selectedLightIndex].param1.z -= 0.05f;
        }
        if (key == GLFW_KEY_8) {
            ::g_pLights->theLights[::g_selectedLightIndex].param1.z += 0.05f;
        }


        //if (key == GLFW_KEY_O) 
        //{
        //    // HACK: GOTCHA
        //    ::g_selectedLightIndex++;
        //}

        if (key == GLFW_KEY_9)
        {
            g_ShowLightDebugSphereThings = false;
        }
        if (key == GLFW_KEY_0) {
            g_ShowLightDebugSphereThings = true;
        }

    }//if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)


    // Move a mesh in the scene
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

        if ( (key == GLFW_KEY_O) && (action == GLFW_PRESS) )
        {
            // TODO: ERROR here if out of range
            if (::g_selectedObjectIndex == 0)
            {
                // Yes, so if we -- it, we're screwed.
                // Set it the LAST index in the vector
                ::g_selectedObjectIndex = (unsigned int)(::g_MeshesToDraw.size() - 1);
            }
            else
            {
                // We can --
                ::g_selectedObjectIndex--;  
            }
        }
        if ( (key == GLFW_KEY_P) && (action == GLFW_PRESS) )  // orGLFW_RELEASE
        {
            ::g_selectedObjectIndex++;

            // To high? 
            if (::g_selectedObjectIndex >= ::g_MeshesToDraw.size() )
            {
                // Yeah, so clamp it? Wrap around?
                ::g_selectedObjectIndex = 0;
            }

            std::cout << "Selected object now: " << ::g_selectedObjectIndex << std::endl;;
        }

        if ( key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        {
            std::ofstream mySaveFileVariable("michaels_scene.scene");

            mySaveFileVariable << ::g_MeshesToDraw.size() << std::endl;

            //std::fstream 

            // theFile << ::g_MeshesToDraw.size() << " "
            // 
            // Save the "scene"
//            for ( cMeshObject* pCM : ::g_MeshesToDraw )
//            for ( std::vector< cMeshObject* >::iterator itCurMesh = ::g_MeshesToDraw.begin();
//                  itCurMesh != ::g_MeshesToDraw.end(); itCurMesh++ )
            for ( int index = 0; index != ::g_MeshesToDraw.size(); index++ )
            {
                std::cout << ::g_MeshesToDraw[index]->meshFileName << std::endl;
                std::cout << ::g_MeshesToDraw[index]->position.x << std::endl;
                std::cout << ::g_MeshesToDraw[index]->position.y << std::endl;
                std::cout << ::g_MeshesToDraw[index]->position.z << std::endl;


                // 8, 7, -12, 23, 33, 

                mySaveFileVariable << ::g_MeshesToDraw[index]->meshFileName << std::endl;
                mySaveFileVariable << ::g_MeshesToDraw[index]->position.x << std::endl;
                mySaveFileVariable << ::g_MeshesToDraw[index]->position.y << std::endl;
                mySaveFileVariable << ::g_MeshesToDraw[index]->position.z << std::endl;


//                cMeshObject a;                          // STACK
//                cMeshObject* pB = new cMeshObject();    // HEAP
//                a.meshFileName = "";
//                pB->meshFileName = "";

                // Barf the object properties to a file
                // theFile << pCM->meshName << " "
                // theFile << pCM->position.x
                // theFile << pCM->position.y
                // .. and so on
            }// Jump

            mySaveFileVariable.close();

            int index = 0;
            do 
            {

            } while (index < ::g_MeshesToDraw.size());

        }
        if ( key == GLFW_KEY_L && action == GLFW_PRESS)
        {
            std::ifstream mySaveFileVariable("michaels_scene.scene");
            // Did it open?
            if ( ! mySaveFileVariable.is_open() )
            {
                std::cout << "Didn't open scene file!" << std::endl;
                return;
            }

            // Rememmber this?
            //cMeshObject* pCow = new cMeshObject();
            //pCow->meshFileName = "assets/models/cow_xyz_n_rgba.ply";
            //pCow->bIsWireFrame = false;
            //pCow->position.x = -10.f;
            //::g_MeshesToDraw.push_back(pCow);

//            ::g_MeshesToDraw.clear();

            int numberOfMeshsesInScene = 0;
            //std::cin >> numberOfMeshsesInScene;
            mySaveFileVariable >> numberOfMeshsesInScene;

            for (int index = 0; index != numberOfMeshsesInScene; index++)
            {
                cMeshObject* pNewObject = new cMeshObject();
//                std::cin >> pNewObject->meshFileName;
//                std::cin >> pNewObject->position.x;
//                std::cin >> pNewObject->position.y;
//                std::cin >> pNewObject->position.z;

                mySaveFileVariable >> pNewObject->meshFileName;
                mySaveFileVariable >> pNewObject->position.x;
                mySaveFileVariable >> pNewObject->position.y;
                mySaveFileVariable >> pNewObject->position.z;

                ::g_MeshesToDraw.push_back(pNewObject);
            }

            mySaveFileVariable.close();
        }

 


    }//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)




    // Are all modifiers up?
   if ( ! areAnyModifiersDown(mods) )
   {
        //enum eState
        //{
        //    SUNSET,         // Is getting darker
        //    SUNRISE,        // Is getting brighter
        //    IS_DAY,         // Is full day (brightest)
        //    IS_NIGHT        // Is night time (darkest)
        //};
       if (key == GLFW_KEY_T /*&& action == GLFW_KEY_DOWN*/ )
       {
           // See what state we are in
           switch (::g_DayNightState )
           {
           case SUNSET:
               break;
           case SUNRISE:
               break;
           case IS_DAY:
               ::g_DayNightState = SUNSET;
               break;
           case IS_NIGHT:
               ::g_DayNightState = SUNRISE;
               break;
           }

       }// if (key == GLFW_KEY_T

//
//       // Move the camera 
//       if (key == GLFW_KEY_A)
//       {
//           // Go left
//           ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
//   //        ::g_cameraTarget.x -= CAMERA_SPEED;
//       }
//       if (key == GLFW_KEY_D)
//       {
//           // Go right
//           ::g_cameraEye.x += CAMERA_MOVE_SPEED;
//   //        ::g_cameraTarget.x += CAMERA_SPEED;
//       }
//
//       if (key == GLFW_KEY_W)
//       {
//           // Go forward
//           ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
//   //        ::g_cameraTarget.x -= CAMERA_SPEED;
//       }
//       if (key == GLFW_KEY_S)
//       {
//           // Go back
//           ::g_cameraEye.z += CAMERA_MOVE_SPEED;
//   //        ::g_cameraTarget.x += CAMERA_SPEED;
//       }
//
//       if (key == GLFW_KEY_Q)
//       {
//           // Go down
//           ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
//   //        ::g_cameraTarget.x -= CAMERA_SPEED;
//       }
//       if (key == GLFW_KEY_E)
//       {
//           // Go up
//           ::g_cameraEye.y += CAMERA_MOVE_SPEED;
//   //        ::g_cameraTarget.x += CAMERA_SPEED;
//       }


   }// if ( ! areAnyModifiersDown(mods) )
    return;
}
