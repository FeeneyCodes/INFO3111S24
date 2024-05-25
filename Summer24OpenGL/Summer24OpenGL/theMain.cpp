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

#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"


#include <vector>
#include "cMeshObject.h"


glm::vec3 g_cameraEye = glm::vec3(0.0, 0.0, -20.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0, 0.0, 0.0f);

extern unsigned int g_selectedObjectIndex;

void LoadFilesIntoVAOManager(GLuint program);
// note that this is a pointer because 

std::vector< cMeshObject* > g_MeshesToDraw;
void LoadModelsIntoScene(void);

cShaderManager* g_pTheShaderManager = NULL;     // Actual thing is on the HEAP
//cShaderManager TheShaderManager;                // Stack

cVAOManager* g_pMeshManager = NULL;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//     void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


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

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);



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


 

    // Load all the TYPES of models I can draw
    ::g_pMeshManager = new cVAOManager();

    // This is the selection of models we CAN 'draw' on the screen
    LoadFilesIntoVAOManager(program);

    // Here's the things we'd like to 'show' in our 'scene
    LoadModelsIntoScene();



    // Choose the shader program we're using
    glUseProgram(program);

    // Enable the depth text per pixel
    glEnable(GL_DEPTH_TEST);

    // Don't draw the "back" part of the objects.
//    glCullFace(GL_BACK);
   
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

//        glm::vec3 cameraEye = glm::vec3(   0.0,   0.0, -20.0f);
//        glm::vec3 cameraTarget = glm::vec3(0.0f, -5.0f, 20.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        matView = glm::lookAt(::g_cameraEye,
                              ::g_cameraTarget,
                              upVector);

        GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
        glUniform3f(eyeLocation_UL, 
                    ::g_cameraEye.x,
                    ::g_cameraEye.y,
                    ::g_cameraEye.z);

       //mat4x4_mul(mvp, p, m);
//        mvp = p * v * m;


        // Start drawing the scene
        // 
        // i.e. draw all the things listed in this:
        // std::vector< cMeshObject* > g_MeshesToDraw;
        //
        for ( unsigned int index = 0; index != ::g_MeshesToDraw.size(); index++ )
        {

            cMeshObject* pCurrentMesh = ::g_MeshesToDraw[index];

            // Is it visible?
            if ( ! pCurrentMesh->bIsVisible ) 
            {
                // Skip it
                continue;
            }


       //         mat4x4_identity(m);
            glm::mat4 matModel = glm::mat4(1.0f);

            glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
                                                      pCurrentMesh->position);

            // Euler axes
            glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
                                               pCurrentMesh->orientation.x,
                                               glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
                                               pCurrentMesh->orientation.y,
                                               glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
                                               pCurrentMesh->orientation.z,
                                               glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 matScaleXYZ = glm::scale(glm::mat4(1.0f), 
                                               glm::vec3(pCurrentMesh->scale,
                                                         pCurrentMesh->scale,
                                                         pCurrentMesh->scale));

            // The order of these is important
            // 1 * 4 * 12 * 3 = 12 * 4 * 12 * 3
            matModel = matModel * matTranslation;

            matModel = matModel * matRotateX;
            matModel = matModel * matRotateY;
            matModel = matModel * matRotateZ;

            matModel = matModel * matScaleXYZ;


            ////mat4x4_rotate_Z(m, m, (float) glfwGetTime());
            //glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
            //                                (float)glfwGetTime(),
            //                                glm::vec3(0.0f, 0.0, 1.0f));

    //        m = m * rotateZ;



            //glUseProgram(program);

            // GL_LINE gives you "wireframe"
            // GL_FILL is default (fully rendered)
            if ( pCurrentMesh->bIsWireFrame )
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
//            glPointSize(10);
            


            GLint mProj_location = glGetUniformLocation(program, "mProj");
            GLint mView_location = glGetUniformLocation(program, "mView");
            GLint mModel_location = glGetUniformLocation(program, "mModel");

            glUniformMatrix4fv(mProj_location, 1, GL_FALSE, 
                               glm::value_ptr(matProjection) );

            glUniformMatrix4fv(mView_location, 1, GL_FALSE,
                               glm::value_ptr(matView) );

            glUniformMatrix4fv(mModel_location, 1, GL_FALSE,
                               glm::value_ptr(matModel) );

            // We need this for lighting
            // uniform mat4 mModel_InverseTranspose;
            GLint mModelIT_location = glGetUniformLocation(program, "mModel_InverseTranspose");

            // Calculate the "inverse transpose of the model matrix"
            glm::mat4 matModelIT = glm::inverse(glm::transpose(matModel));

            glUniformMatrix4fv(mModelIT_location, 1, GL_FALSE,
                               glm::value_ptr(matModelIT) );


            // Do I override the vertex colour
            GLint colourOverride_UL = glGetUniformLocation(program, "colourOverride");
            GLint bUseOverrideColour_UL = glGetUniformLocation(program, "bUseOverrideColour");

            if ( pCurrentMesh->bOverrideVertexModelColour )
            {
                glUniform3f(colourOverride_UL,              // uniform vec3 colourOverride;	
                            pCurrentMesh->colourRGB.r,
                            pCurrentMesh->colourRGB.g,
                            pCurrentMesh->colourRGB.b);

                // All types are really floats, so a bool is really a single float
                glUniform1f(bUseOverrideColour_UL, (GLfloat)GL_TRUE);       // or 1.0
            }
            else
            {
                // All types are really floats, so a bool is really a single float
                glUniform1f(bUseOverrideColour_UL, (GLfloat)GL_FALSE);       // or 0
            }



            sModelDrawInfo modelInfo;
            if (::g_pMeshManager->FindDrawInfoByModelName(pCurrentMesh->meshFileName, modelInfo))
            {
                // Found it!
                glBindVertexArray(modelInfo.VAO_ID);

                glDrawElements(GL_TRIANGLES,
                               modelInfo.numberOfIndices,
                               GL_UNSIGNED_INT,
                               (void*)0);

                glBindVertexArray(0);
            }

        }//for ( unsigned int index 


         
         
         // Update the window title to show where the camera is...
        std::stringstream ssWindowsTitle;
//        std::cout 
        ssWindowsTitle
            << "Camera (xyz)"
            << ::g_cameraEye.x << ", "
            << ::g_cameraEye.y << ", "
            << ::g_cameraEye.z 
            << "  " 
            << "selected object ID: " 
            << ::g_selectedObjectIndex;

//        glfwSetWindowTitle(window, "HEY!");
        glfwSetWindowTitle(window, ssWindowsTitle.str().c_str() );


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void LoadFilesIntoVAOManager(GLuint program)
{
    sModelDrawInfo meshInfoCow;
    if (!::g_pMeshManager->LoadModelIntoVAO("assets/models/cow_xyz_n_rgba.ply", meshInfoCow, program))
    {
        std::cout << "ERROR: Didn't load the cow" << std::endl;
    }

    sModelDrawInfo carMeshInfo;
    if (!::g_pMeshManager->LoadModelIntoVAO("assets/models/de--lorean_xyz_n_rgba.ply", carMeshInfo, program))
    {
        std::cout << "ERROR: Didn't load the cow" << std::endl;
    }

    sModelDrawInfo teapotMeshInfo;
    if (!::g_pMeshManager->LoadModelIntoVAO("assets/models/Utah_Teapot_xyz_rgba.ply", teapotMeshInfo, program))
    {
        std::cout << "ERROR: Didn't load the cow" << std::endl;
    }

    sModelDrawInfo dolphinMesh;
    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/dolphin_xyz_n_rgba.ply", dolphinMesh, program))
    {
        std::cout << "loaded: "
            << dolphinMesh.meshName << " "
            << dolphinMesh.numberOfVertices << " vertices" << std::endl;
    }

    sModelDrawInfo terrainMesh;
    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/fractalTerrainMeshLab_xyz_n_rgba.ply", terrainMesh, program))
    {
        std::cout << "loaded: "
            << terrainMesh.meshName << " "
            << terrainMesh.numberOfVertices << " vertices" << std::endl;
    }


    sModelDrawInfo wearhouseMesh;
    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Warehouse_xyz_n_rgba.ply", wearhouseMesh, program))
    {
        std::cout << "loaded: "
            << wearhouseMesh.meshName << " "
            << wearhouseMesh.numberOfVertices << " vertices" << std::endl;
    }

    return;
}

void LoadModelsIntoScene(void)
{
        // Load the models I'd like to draw in the scene
    cMeshObject* pCow = new cMeshObject();
    pCow->meshFileName = "assets/models/cow_xyz_n_rgba.ply";
    pCow->bIsWireFrame = false;
    pCow->position.x = -10.f;
    ::g_MeshesToDraw.push_back(pCow);

    cMeshObject* pCow2 = new cMeshObject();
    pCow2->meshFileName = "assets/models/cow_xyz_n_rgba.ply";
//    pCow2->bIsWireFrame = false;
    pCow2->position.x = +10.0f;
    pCow2->scale = 0.5f;
    pCow->orientation.z = glm::radians(-45.0f);
    pCow2->bOverrideVertexModelColour = true;
    pCow2->colourRGB = glm::vec3(0.0f, 1.0f, 0.0f);
    ::g_MeshesToDraw.push_back(pCow2);

    cMeshObject* pCar = new cMeshObject();
    pCar->meshFileName = "assets/models/de--lorean_xyz_n_rgba.ply";
    pCar->orientation.x = glm::radians(-90.0f);
    pCar->position.z = 25.0f;
//    pCar->bIsWireFrame = true;
    pCar->bIsVisible = false;
    ::g_MeshesToDraw.push_back(pCar);

    cMeshObject* pDolphin = new cMeshObject();
    pDolphin->meshFileName = "assets/models/dolphin_xyz_n_rgba.ply";
    pDolphin->scale = 0.01f;
    ::g_MeshesToDraw.push_back(pDolphin);

    cMeshObject* pTerrain = new cMeshObject();
    pTerrain->meshFileName = "assets/models/fractalTerrainMeshLab_xyz_n_rgba.ply";
    pTerrain->position.y = -30.0f;
//    pTerrain->bIsWireFrame = true;
    ::g_MeshesToDraw.push_back(pTerrain);

    cMeshObject* pWarehouse = new cMeshObject();
    pWarehouse->meshFileName = "assets/models/Warehouse_xyz_n_rgba.ply";
//    pWarehouse->bIsWireFrame = true;
    pWarehouse->position.y = -10.0f;
    pWarehouse->orientation.y = glm::radians(-90.0f);
    ::g_MeshesToDraw.push_back(pWarehouse);

    return;
}
