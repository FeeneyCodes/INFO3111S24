//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "globalOpenGL.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cLightManager.h"
#include "cLightHelper/cLightHelper.h"

#include "cBasicTextureManager/cBasicTextureManager.h"

#include <vector>
#include "cMeshObject.h"

#include "sharedThings.h"

extern unsigned int g_selectedLightIndex;

// This will be our "skybox" object
extern cMeshObject* g_pSkyBoxSphere;// = NULL;


//glm::vec3 g_cameraEye = glm::vec3(0.0, 12.0f, -39.0f);
//glm::vec3 g_cameraTarget = glm::vec3(0.0, 0.0, 0.0f);
cBasicFlyCamera* g_pFlyCamera = NULL;

extern unsigned int g_selectedObjectIndex;

void LoadFilesIntoVAOManager(GLuint program);
// note that this is a pointer because 

//std::vector< cMeshObject* > g_MeshesToDraw;
void LoadModelsIntoScene(void);

void LoadTextures(cBasicTextureManager* pTextureManager);


cShaderManager* g_pTheShaderManager = NULL;     // Actual thing is on the HEAP
//cShaderManager TheShaderManager;                // Stack

cVAOManager* g_pMeshManager = NULL;

cBasicTextureManager* g_TextureManager = NULL;

cLightManager* g_pLights = NULL;

// This is some object we can move about
cMeshObject* g_pSmoothSphere = NULL;

extern bool g_ShowLightDebugSphereThings;

void handleKeyboardAsync(GLFWwindow* window);
void handleMouseAsync(GLFWwindow* window);


cMeshObject* g_findMeshByFriendlyName( std::string theName )
{
    // can also be a regular for loop
    for ( cMeshObject* pCurMesh : g_MeshesToDraw )
    {
        if ( pCurMesh->friendlyName == theName )
        {
            return pCurMesh;
        }
    }
    return NULL;    // or nullptr or 0
}


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//     void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Set the callbacks for the mouse
// https://www.glfw.org/docs/3.3/input_guide.html#input_mouse

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// Set with glfwSetCursorPosCallback(window, cursor_position_callback);

void cursor_enter_callback(GLFWwindow* window, int entered);
// Set with glfwSetCursorEnterCallback(window, cursor_enter_callback);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// Set with glfwSetMouseButtonCallback(window, mouse_button_callback);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// Set with glfwSetScrollCallback(window, scroll_callback);


void DrawMesh(cMeshObject* pCurrentMesh, GLuint shaderProgram);

// HACK:
float cameraRotationAngleChangePerFrame = 0.1f;

int main(void)
{
 //   MessageBox(NULL, L"I love OpenGL", L"Hey!", MB_OK);

//    int ax[3] = { 1, 2, 3 };    // Stack (at compile time)
//    int* pAX = new int[3];      // Heap (at run time)
    // Copy the data

    GLFWwindow* window;
//    GLuint vertex_buffer;
//    GLuint vertex_shader;
//    GLuint fragment_shader;
    GLuint program = 0;
//    GLint mvp_location;

    //GLint vpos_location;
    //GLint vcol_location;


    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the camrea BEFORE connecting the mouse and keyboard callbacks
    ::g_pFlyCamera = new cBasicFlyCamera();

    ::g_pFlyCamera->setEyeLocation(0.0f, 0.0f, +20.0f);


    glfwSetKeyCallback(window, key_callback);

    // Also set the mouse callback functions
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);


    if (glfwRawMouseMotionSupported())
    {
        std::cout << "DOES handle raw mouse motion" << std::endl;
        
        //glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    else
    {
        std::cout << "Does NOT handle raw mouse motion" << std::endl;
    }



    // int x = 0;
    // int* pX = new int(); // Same thing
    ::g_pTheShaderManager = new cShaderManager();

    cShaderManager::cShader vertShader;
    vertShader.fileName = "vertexShader01.glsl";

    cShaderManager::cShader fragShader;
    fragShader.fileName = "fragShader01.glsl";

    ::g_pTheShaderManager->setBasePath("assets/shaders/");

    if ( ::g_pTheShaderManager->createProgramFromFile("shader1", vertShader, fragShader ) )
    {
        std::cout << "Hazzah! Compiled OK" << std::endl;
    }
    else
    {
        std::cout << "Shader creation error: "
            << ::g_pTheShaderManager->getLastError()
            << std::endl;
    }

    // Set the shader to the one we want
    program = ::g_pTheShaderManager->getIDFromFriendlyName("shader1");

    // Choose the shader program we're using
    glUseProgram(program);

//    glDeleteShader(program);

 

    // Load all the TYPES of models I can draw
    ::g_pMeshManager = new cVAOManager();

    // This is the selection of models we CAN 'draw' on the screen
    LoadFilesIntoVAOManager(program);

    // Here's the things we'd like to 'show' in our 'scene
    LoadModelsIntoScene();

    ::g_TextureManager = new cBasicTextureManager();

    LoadTextures(::g_TextureManager);





    // Enable the depth text per pixel
    glEnable(GL_DEPTH_TEST);

    // Don't draw the "back" part of the objects.
//    glCullFace(GL_BACK);


    ::g_pFlyCamera->setEyeLocation(38.4765f, 47.9442f, 41.7282f);


    // Create the "light manager"
    ::g_pLights = new cLightManager();

    // Get the uniform locations of all the params of the lights
    ::g_pLights->GetUniformLocations(program);

    std::cout << cLightManager::NUMBEROFLIGHTS << std::endl;
    std::cout << ::g_pLights->NUMBEROFLIGHTS << std::endl;

    // 
    ::g_pLights->theLights[0].position = glm::vec4(0.0f, 41.5f, 98.0f, 98.0f);
    // White light (flourescent) light
//    ::g_pLights->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // https://www.google.com/search?q=rgb+of+sunlight&oq=rgb+of+sunli&gs_lcrp=EgZjaHJvbWUqBwgAEAAYgAQyBwgAEAAYgAQyBggBEEUYOTINCAIQABiGAxiABBiKBTINCAMQABiGAxiABBiKBTINCAQQABiGAxiABBiKBTINCAUQABiGAxiABBiKBTIKCAYQABiABBiiBKgCALACAQ&sourceid=chrome&ie=UTF-8
    //255, 222, 131
    //::g_pLights->theLights[0].diffuse = glm::vec4(255.0f/255.0f, 
    //                                              222.0f/255.0f, 
    //                                              131.0f/255.0f, 
    //                                              1.0f);
    ::g_pLights->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 
                                                  1.0f);
    // Turn on
//    ::g_pLights->theLights[0].turnOn();
    ::g_pLights->theLights[0].param2.x = 1.0f;  // Turn on
    // Set the attenuation
    ::g_pLights->theLights[0].atten.x = 0.0f;   // Constant
    ::g_pLights->theLights[0].atten.y = 0.0333305f;   // Linear
    ::g_pLights->theLights[0].atten.z = 0.0002894f;   // Quadratic

    //
    ::g_pLights->theLights[0].param1.x = 0.0f;  // Point light
//    ::g_pLights->theLights[0].param1.x = 1.0f;  // Spot light
    // 
    ::g_pLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
//   vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//               // 0 = pointlight
//               // 1 = spot light
//               // 2 = directional light
    ::g_pLights->theLights[0].param1.y = 15.0f;
    ::g_pLights->theLights[0].param1.z = 25.0f;



       // 
    ::g_pLights->theLights[1].position = glm::vec4(-13.0f, 5.0f, 0.0f, 1.0f);
    ::g_pLights->theLights[1].diffuse = glm::vec4(0.25f, 1.0f, 0.25f, 1.0f);
    //
    ::g_pLights->theLights[1].param1.x = 0.0f;  // Point light
    // Turn on
    ::g_pLights->theLights[1].param2.x = 0.0f;  // Turn on
    // Set the attenuation
    ::g_pLights->theLights[1].atten.x = 0.0f;   // Constant
    ::g_pLights->theLights[1].atten.y = 0.02f;   // Linear
    ::g_pLights->theLights[1].atten.z = 0.005f;   // Quadratic

       // 
    ::g_pLights->theLights[2].position = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
    ::g_pLights->theLights[2].diffuse = glm::vec4(0.25f, 0.25f, 1.0f, 1.0f);
    //
    ::g_pLights->theLights[2].param1.x = 0.0f;  // Point light
    // Turn on
    ::g_pLights->theLights[2].param2.x = 0.0f;  // Turn on
    // Set the attenuation
    ::g_pLights->theLights[2].atten.x = 0.0f;   // Constant
    ::g_pLights->theLights[2].atten.y = 0.02f;   // Linear
    ::g_pLights->theLights[2].atten.z = 0.005f;   // Quadratic


//    // Directional light BEWARE!!!!
//    ::g_pLights->theLights[3].param1.x = 2.0f;  // const int DIRECTIONAL_LIGHT_TYPE = 2;
//    ::g_pLights->theLights[3].direction = glm::vec4(0.9f, -0.1f, 0.0f, 1.0f);
//    ::g_pLights->theLights[3].turnOn();
//    ::g_pLights->theLights[3].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
//    // Turn the other ones off
//    ::g_pLights->theLights[0].turnOff();
//    ::g_pLights->theLights[1].turnOff();
//    ::g_pLights->theLights[2].turnOff();

    // Main loop runs forever
    while ( ! glfwWindowShouldClose(window) )
    {
        float ratio;
        int width, height;
 //       mat4x4 m, p, mvp;
 //       glm::mat4 m, p, v, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glm::mat4 matProjection = 
            glm::perspective(0.6f,          // FOV
                             ratio,         // screen aspect ratio 16:9
                             1.0f,          // Neap plane
                             10'000.0f);      // Far plane

        // float 6-7

        glm::mat4 matView = glm::mat4(1.0f);


//        // Pivot the camera
//        cMeshObject* pCameraModel = ::g_findMeshByFriendlyName("Camera");
//        cMeshObject* pCameraTargetBall = ::g_findMeshByFriendlyName("CameraTargetBall");
//        if (pCameraModel && pCameraTargetBall)
//        {
//            pCameraModel->orientation.y += glm::radians(cameraRotationAngleChangePerFrame);
//            pCameraTargetBall->orientation.y += glm::radians(cameraRotationAngleChangePerFrame);
//
//            // Check limits
//            if (pCameraModel->orientation.y >= glm::radians(45.0f))
//            {
//                cameraRotationAngleChangePerFrame = -0.1f;
//            }
//            if (pCameraModel->orientation.y <= glm::radians(-45.0f))
//            {
//                cameraRotationAngleChangePerFrame = +0.15f;
//            }
//
////            // Set the eye where the camera is
////            ::g_cameraEye = pCameraModel->position;
////            // Set the target (at) where the sphere (in front of the camera) is
////            ::g_cameraTarget = pCameraTargetBall->position;
//        }


//        glm::vec3 cameraEye = glm::vec3(   0.0,   0.0, -20.0f);
//        glm::vec3 cameraTarget = glm::vec3(0.0f, -5.0f, 20.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        //matView = glm::lookAt(::g_cameraEye,
        //                      ::g_cameraTarget,
        //                      upVector);
        matView = glm::lookAt(::g_pFlyCamera->getEyeLocation(),
                              ::g_pFlyCamera->getTargetLocation(),
                              upVector);

        GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
        //glUniform3f(eyeLocation_UL, 
        //            ::g_cameraEye.x,
        //            ::g_cameraEye.y,
        //            ::g_cameraEye.z);
        glUniform3f(eyeLocation_UL, 
                    ::g_pFlyCamera->getEyeLocation().x,
                    ::g_pFlyCamera->getEyeLocation().y,
                    ::g_pFlyCamera->getEyeLocation().z);



       //mat4x4_mul(mvp, p, m);
//        mvp = p * v * m;


        // Blend from wall to Beyonce
        cMeshObject* pGround = g_findMeshByFriendlyName("The Ground");
        if ( pGround )
        {
            if (pGround->textureBlendRatio[0] >= 0.0f )
            {
                // It's there
                pGround->textureBlendRatio[0] -= 0.0001f;
                pGround->textureBlendRatio[1] += 0.0001f;
            }
        }



        // aim the spotlight at the cow
        cMeshObject* pGeorge = ::g_findMeshByFriendlyName("George");
        if ( pGeorge /* != NULL */ )
        {

            glm::vec3 lightPosition = glm::vec3(::g_pLights->theLights[0].position);

            glm::vec3 cowPosition = pGeorge->position;

            glm::vec3 ligthToCowVector = cowPosition - lightPosition;

            // Normalizing means divide the ray by it's length
            glm::vec3 ligthToCowVectorNormalized = glm::normalize(ligthToCowVector);

            ::g_pLights->theLights[0].direction = glm::vec4(ligthToCowVectorNormalized, 1.0f);
        }


        // Place light #0 where the sphere is
//        ::g_pLights->theLights[0].position = glm::vec4( g_pSmoothSphere->position, 1.0f);
        ::g_pSmoothSphere->position = glm::vec3(::g_pLights->theLights[0].position);

        // Copy the light information to the shader
        ::g_pLights->UpdateShaderUniforms(program);

        // The project and view stay the same for the entire scene 
        // (like they stay the same as we draw all the objects this pass (or frame))
        GLint mProj_location = glGetUniformLocation(program, "mProj");
        GLint mView_location = glGetUniformLocation(program, "mView");

        glUniformMatrix4fv(mProj_location, 1, GL_FALSE,
                           glm::value_ptr(matProjection));

        glUniformMatrix4fv(mView_location, 1, GL_FALSE,
                           glm::value_ptr(matView));


        // Turn on the lighting
        // uniform bool bDoNotLight;
        GLint bDoNotLight_UL = glGetUniformLocation(program, "bDoNotLight");
        glUniform1f(bDoNotLight_UL, (GLint)GL_FALSE);        // Pass 1.0f;

        GLint ml_UL = glGetUniformLocation(program, "maxLightIndexInUse");
        glUniform1i(ml_UL, cLightManager::NUMBEROFLIGHTS);

        // Start drawing the scene
        // 
        // i.e. draw all the things listed in this:
        // std::vector< cMeshObject* > g_MeshesToDraw;
        //
        // NON-transparent thing...

        std::vector<cMeshObject*> vecTransparentThings;
        std::vector<cMeshObject*> vecSolidThings;

        for ( cMeshObject* pCurMesh : ::g_MeshesToDraw )
        {
            if (pCurMesh->alphaTransparency < 1.0f )
            {
                vecTransparentThings.push_back(pCurMesh);
            }
            else
            {
                vecSolidThings.push_back(pCurMesh);
            }
        }

        // Sort the transparent things far to near

        // Vector of 100 to 1000 thigns
        // Faster search...?
        // LINEAR search

//        Pentium 586 --> Quake 3
//        CPU SPEED : Intel 266 MHz / AMD 350 MHz.
//        RAM : 64 MB.


        // 100 object at 4GHz
        // 4,000,000,000 per second @ 60Hz
        //    40,000,000 per frame
        // 10,000 until they are in order
        //    4,000x per frame


        //vectro
        //list
        //hash map
        //map (red-black tree)


        for ( unsigned int index = 0; index != vecSolidThings.size(); index++ )
        {

            cMeshObject* pCurrentMesh = vecSolidThings[index];


            DrawMesh(pCurrentMesh, program);

        }//for ( unsigned int index 

        for ( unsigned int index = 0; index != vecTransparentThings.size(); index++ )
        {

            cMeshObject* pCurrentMesh = vecTransparentThings[index];


            DrawMesh(pCurrentMesh, program);

        }//for ( unsigned int index 

//        DrawMesh(::g_pSmoothSphere, program);
// 

        //for ( float x = -50.0f; x < 50.0f; x += 5.0f )
        //{
        //    ::g_MeshesToDraw[1]->scale = 5.0f;
        //    ::g_MeshesToDraw[1]->position.x = x;
        //    ::g_MeshesToDraw[1]->position.y = 10.0f;
        //    ::g_MeshesToDraw[1]->position.z = 0.0f;
        //    ::g_MeshesToDraw[1]->colourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
        //    DrawMesh(::g_MeshesToDraw[1], program);
        //}


        // Draw the skybox
        if ( ::g_pSkyBoxSphere )
        {
            // Turn "on" the skybox
            // uniform bool bIsSkyBox;
            GLint bIsSkyBox_UL = glGetUniformLocation(program, "bIsSkyBox");
            glUniform1f(bIsSkyBox_UL, (GLfloat)GL_TRUE);

            // Set the skybox texture, too
            // uniform samplerCube skyBoxTexture;

            // EXACTLY the same as a 2D texture, except for 
            // the GL_TEXTURE_CUBE_MAP when binding

            GLuint skyboxTextureID = ::g_TextureManager->getTextureIDFromName("SunnyDay");
//            GLuint skyboxTextureID = ::g_TextureManager->getTextureIDFromName("Space");
            GLuint texture30Unit = 30;			// Texture unit go from 0 to 79
            glActiveTexture(GL_TEXTURE0 + texture30Unit);	// GL_TEXTURE0 = 33984
            // NOTE: This isn't a GL_TEXTURE_2D
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
            GLint skyBoxTexture_UL = glGetUniformLocation(program, "skyBoxTexture");
            glUniform1i(skyBoxTexture_UL, texture30Unit);

            // Two ways:
            // 1. (correct, bougie way)
            //      Turn off the depth buffer comparison
            //      Tuen off writing to the depth buffer
            //      Move the sphere to the camera
            //      Draw the teeny weeny sphere 
            //      Turn everything back on 
            //      Render as usual
            //
            // 2. Move the sphere to the camera
            //    Make it big enough to hold everything
            //    (like in the horizon)

            ::g_pSkyBoxSphere->scale = 5'000.0f;
            ::g_pSkyBoxSphere->position = ::g_pFlyCamera->getEyeLocation();

            DrawMesh(::g_pSkyBoxSphere, program);

            // Turn "off" the skybox rendering
            glUniform1f(bIsSkyBox_UL, (GLfloat)GL_FALSE);
        }


//        L:0.00249			Light
//        Q:0.00028
//        L:0.057			Dark
//        Q:0.000821
        float numFrames = 60.0f * 60.0f * 5.0f; // 60FPS * 5 seconds
        float linearAttenChange = (0.057f - 0.00249f) / numFrames;
        float quadAttenChange = (0.000821f - 0.00028f) / numFrames;

        // Handle the day night cycle
        // See what state we are in
        switch (::g_DayNightState)
        {
        case SUNSET:
            // Make it darker
            ::g_pLights->theLights[0].atten.y += linearAttenChange;
            ::g_pLights->theLights[0].atten.z += quadAttenChange;
            // Dark enough? 
            if (::g_pLights->theLights[0].atten.y >= 0.057f)
            {
                ::g_DayNightState = IS_NIGHT;
            }
            break;
        case SUNRISE:
            // Make it lighter
            ::g_pLights->theLights[0].atten.y -= linearAttenChange;
            ::g_pLights->theLights[0].atten.z -= quadAttenChange;
            if (::g_pLights->theLights[0].atten.y <= 0.00249f)
            {
                ::g_DayNightState = IS_DAY;
            }
            break;
        //case IS_DAY:
        //    ::g_DayNightState = SUNSET;
        //    break;
        //case IS_NIGHT:
        //    ::g_DayNightState = SUNRISE;
        //    break;
        }




        if (g_ShowLightDebugSphereThings)
        {

            // Draw concentric sphers based on how bright the light is at 'that' distance
            cLightHelper myLH;

            const float errorValueForLightLevelGuess = 0.001f;
            const float INFINITE_DISTANCE = 100000.0f;

            // Turn off the lights
            glUniform1f(bDoNotLight_UL, (GLint)GL_TRUE);        // Pass 1.0f;

            // The centre of the light
            ::g_pSmoothSphere->scale = 0.1f;
            ::g_pSmoothSphere->colourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
            ::g_pSmoothSphere->bOverrideVertexModelColour = true;
            ::g_pSmoothSphere->bDoNotLight = true;
            ::g_pSmoothSphere->bUseTextureAsColour = false;
            ::g_pSmoothSphere->position = ::g_pLights->theLights[::g_selectedLightIndex].position;
            DrawMesh(::g_pSmoothSphere, program);

            float distanceAt75Percent =
                myLH.calcApproxDistFromAtten(0.75f,
                                             errorValueForLightLevelGuess,
                                             INFINITE_DISTANCE,
                                             ::g_pLights->theLights[::g_selectedLightIndex].atten.x,     // Const
                                             ::g_pLights->theLights[::g_selectedLightIndex].atten.y,     // Linear
                                             ::g_pLights->theLights[::g_selectedLightIndex].atten.z);    // Quad

            ::g_pSmoothSphere->scale = distanceAt75Percent;
            ::g_pSmoothSphere->colourRGB = glm::vec3(0.25f, 0.25f, 0.0f);
            DrawMesh(::g_pSmoothSphere, program);


            float distanceAt50Percent = 
                myLH.calcApproxDistFromAtten(0.5f,
                                         errorValueForLightLevelGuess,
                                         INFINITE_DISTANCE,
                                         ::g_pLights->theLights[::g_selectedLightIndex].atten.x,     // Const
                                         ::g_pLights->theLights[::g_selectedLightIndex].atten.y,     // Linear
                                         ::g_pLights->theLights[::g_selectedLightIndex].atten.z);    // Quad

            ::g_pSmoothSphere->scale = distanceAt50Percent;
            ::g_pSmoothSphere->colourRGB = glm::vec3(0.25f, 0.0f, 0.0f);
            DrawMesh(::g_pSmoothSphere, program);


            float distanceAt25Percent =
                myLH.calcApproxDistFromAtten(0.25f,
                                             errorValueForLightLevelGuess,
                                             INFINITE_DISTANCE,
                                             ::g_pLights->theLights[::g_selectedLightIndex].atten.x,     // Const
                                             ::g_pLights->theLights[::g_selectedLightIndex].atten.y,     // Linear
                                             ::g_pLights->theLights[::g_selectedLightIndex].atten.z);    // Quad

            ::g_pSmoothSphere->scale = distanceAt25Percent;
            ::g_pSmoothSphere->colourRGB = glm::vec3(0.0f, 0.25f, 0.0f);
            DrawMesh(::g_pSmoothSphere, program);



                                    
            float distanceAt01Percent = 
            myLH.calcApproxDistFromAtten(0.01f,     // really 'zero'
                                        errorValueForLightLevelGuess,
                                        INFINITE_DISTANCE,
                                        ::g_pLights->theLights[::g_selectedLightIndex].atten.x,     // Const
                                        ::g_pLights->theLights[::g_selectedLightIndex].atten.y,     // Linear
                                        ::g_pLights->theLights[::g_selectedLightIndex].atten.z);    // Quad

            ::g_pSmoothSphere->scale = distanceAt01Percent;
            ::g_pSmoothSphere->colourRGB = glm::vec3(0.0f, 0.25f, 0.5f);
            DrawMesh(::g_pSmoothSphere, program);

            glUniform1f(bDoNotLight_UL, (GLint)GL_FALSE);        // Pass 1.0f;
        }//if (g_ShowLightDebugSphereThings)
         
         // Update the window title to show where the camera is...
        std::stringstream ssWindowsTitle;
//        std::cout 
        ssWindowsTitle
            << "Camera (xyz)"
            << ::g_pFlyCamera->getEyeLocation().x << ", "       // << ::g_cameraEye.x
            << ::g_pFlyCamera->getEyeLocation().y << ", "       // << ::g_cameraEye.y
            << ::g_pFlyCamera->getEyeLocation().z << ", "       // << ::g_cameraEye.z
            << "  "
            << "light#0: "
            << ::g_pLights->theLights[0].position.x << ", "
            << ::g_pLights->theLights[0].position.y << ", "
            << ::g_pLights->theLights[0].position.z
            << " linear: "
            << ::g_pLights->theLights[0].atten.y << "  "        // linear attenuation
            << " quad: "
            << ::g_pLights->theLights[0].atten.z << "  ";        // Quadratic


//            << "selected object ID: " 
//            << ::g_selectedObjectIndex;

//        glfwSetWindowTitle(window, "HEY!");
        glfwSetWindowTitle(window, ssWindowsTitle.str().c_str() );


        glfwSwapBuffers(window);
        glfwPollEvents();

        //
        handleKeyboardAsync(window);
        handleMouseAsync(window);

    }


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


