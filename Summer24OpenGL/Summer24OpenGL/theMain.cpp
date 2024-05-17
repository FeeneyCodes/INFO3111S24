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

// note that this is a pointer because 
std::vector< cMeshObject* > g_MeshesToDraw;


cShaderManager* g_pTheShaderManager = NULL;     // Actual thing is on the HEAP
//cShaderManager TheShaderManager;                // Stack

cVAOManager* g_pMeshManager = NULL;

struct sVertex
{
    float x, y, z;         // Positions
    float r, g, b;          // Colours
};

//sVertex vertices[3] =
//{
//    { -0.6f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f },   // B.R.
//    {  0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f },   // B.L.
//    {  0.0f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f }    // TOP
//};
sVertex* pVertices = NULL;  // 0, nulptr

unsigned int g_NumVerticesToDraw = 0;
unsigned int g_SizeOfVertexArrayInBytes = 0;

// NON-indexed model loader
bool loadPLY_XYZ_Format(std::string fileName)
{
    //2.24562 -0.864372 0.995778 
    struct sPlyVertexXYZ
    {
        float x, y, z;
    };

    //3 1068 1075 1076 
    struct sPlyTriangle
    {
        int numVertices;    // 3
        int v0, v1, v2;
    };

    std::vector<sPlyVertexXYZ> vecVertices;
    std::vector<sPlyTriangle> vecTriangles;

    std::ifstream theFile(fileName.c_str());
    if (!theFile.is_open())
    {
        return false;
    }

    std::string curWord;

    std::cout << "Type in vertex to win!";
    while (theFile >> curWord)
    {
        if (curWord == "vertex")
        {
            break;
        }
    }
    // element vertex 2903
    unsigned int numVertices = 0;
    theFile >> numVertices;


    while (theFile >> curWord)
    {
        if (curWord == "face")
        {
            break;
        }
    }
    // element face 5804
    unsigned int numTriangles = 0;
    theFile >> numTriangles;


    while (theFile >> curWord)
    {
        if (curWord == "end_header")
        {
            break;
        }
    }

    // Start to read the vertices...
    //  4.13317 2.17523 1.25932 
    //  4.23234 1.90308 0.534362
    for (unsigned int count = 0; count < numVertices; count++)
    {
        sPlyVertexXYZ curVert;

        theFile
            >> curVert.x
            >> curVert.y
            >> curVert.z;

        vecVertices.push_back(curVert);
    }

    //  3 250 251 210
    //  3 250 210 252
    for (unsigned int count = 0; count < numTriangles; count++)
    {
        sPlyTriangle curTri;

        theFile
            >> curTri.numVertices       // 3
            >> curTri.v0
            >> curTri.v1
            >> curTri.v2;

        vecTriangles.push_back(curTri);
    }


    // At this point the model is loaded into the vecVertices and vecTriangles


    // Each triangle (primitive) has 3 vertices
    unsigned int numVertices_TO_DRAW = numTriangles * 3;

//    sVertex* pVertices = NULL;  // 0, nulptr
//    pVertices = new sVertex[numVertices];

    pVertices = new sVertex[numVertices_TO_DRAW];


    unsigned int vertIndex = 0;
    for (unsigned int index = 0; index != numTriangles; index++)
    {
        // Copy from the ply type vector to the evil c array 
        //  that the code wants because it'

        // Look up the triangle info
        sPlyTriangle curTri = vecTriangles[index];
            
        // 3 630 541 542 
        sPlyVertexXYZ v0 = vecVertices[curTri.v0];      // vertex # 630
        sPlyVertexXYZ v1 = vecVertices[curTri.v1];      // 541
        sPlyVertexXYZ v2 = vecVertices[curTri.v2];      // 542

        pVertices[vertIndex].x = v0.x;
        pVertices[vertIndex].y = v0.y;
        pVertices[vertIndex].z = v0.z;
        pVertices[vertIndex].r = 0.0f;
        pVertices[vertIndex].g = 1.0f;
        pVertices[vertIndex].b = 0.0f;

        pVertices[vertIndex + 1].x = v1.x;
        pVertices[vertIndex + 1].y = v1.y;
        pVertices[vertIndex + 1].z = v1.z;
        pVertices[vertIndex + 1].r = 0.0f;
        pVertices[vertIndex + 1].g = 1.0f;
        pVertices[vertIndex + 1].b = 0.0f;

        pVertices[vertIndex + 2].x = v2.x;
        pVertices[vertIndex + 2].y = v2.y;
        pVertices[vertIndex + 2].z = v2.z;
        pVertices[vertIndex + 2].r = 0.0f;
        pVertices[vertIndex + 2].g = 1.0f;
        pVertices[vertIndex + 2].b = 0.0f;


        vertIndex += 3; // Next triangle or 3 vertices later
    }

//    ::g_NumVerticesToDraw = numVertices;    // BEWARE!
    ::g_NumVerticesToDraw = numVertices_TO_DRAW;    // BEWARE!

//    ::g_SizeOfVertexArrayInBytes = numVertices * sizeof(sVertex);
    ::g_SizeOfVertexArrayInBytes = numVertices_TO_DRAW * sizeof(sVertex);


    // TODO: Copy from one place to the other..

    theFile.close();

    return true;
}


bool loadPLY_XYZ_confidence_intensity_format(std::string fileName)
{
    //2.24562 -0.864372 0.995778 
    struct sPlyVertexXYZ_CI
    {
        float x, y, z;
        float confidence;
        float intensity;
    };

    //3 1068 1075 1076 
    struct sPlyTriangle
    {
        int numVertices;    // 3
        int v0, v1, v2;
    };

    std::vector<sPlyVertexXYZ_CI> vecVertices;
    std::vector<sPlyTriangle> vecTriangles;

    std::ifstream theFile(fileName.c_str());
    if (!theFile.is_open())
    {
        return false;
    }

    std::string curWord;

    std::cout << "Type in vertex to win!";
    while (theFile >> curWord)
    {
        if (curWord == "vertex")
        {
            break;
        }
    }
    // element vertex 2903
    unsigned int numVertices = 0;
    theFile >> numVertices;


    while (theFile >> curWord)
    {
        if (curWord == "face")
        {
            break;
        }
    }
    // element face 5804
    unsigned int numTriangles = 0;
    theFile >> numTriangles;


    while (theFile >> curWord)
    {
        if (curWord == "end_header")
        {
            break;
        }
    }

    // Start to read the vertices...
    //  4.13317 2.17523 1.25932 
    //  4.23234 1.90308 0.534362
    for (unsigned int count = 0; count < numVertices; count++)
    {
        sPlyVertexXYZ_CI curVert;

        theFile
            >> curVert.x
            >> curVert.y
            >> curVert.z;
        // Also the stuff that's in the bunny rabbit file because who knows?
        theFile
            >> curVert.confidence
            >> curVert.intensity;

        vecVertices.push_back(curVert);
    }

    //  3 250 251 210
    //  3 250 210 252
    for (unsigned int count = 0; count < numTriangles; count++)
    {
        sPlyTriangle curTri;

        theFile
            >> curTri.numVertices       // 3
            >> curTri.v0
            >> curTri.v1
            >> curTri.v2;

        vecTriangles.push_back(curTri);
    }


    // Do some copying to the format that the shader wants...
//    struct sVertex {
//        float x, y, z;         // Positions
//        float r, g, b;          // Colours
//    };
//
//    sVertex vertices[3] = {
//        { -0.6f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f },   // B.R.
//        {  0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f },   // B.L.
//        {  0.0f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f }    // TOP
//    };

//    std::vector< sVertex> vecVerticesLikeTheyAreInTheShader;

//    sVertex* pVertices = NULL;  // 0, nulptr
    pVertices = new sVertex[numVertices];
    for (unsigned int index = 0; index != numVertices; index++)
    {
        // Copy from the ply type vector to the evil c array 
        //  that the code wants because it'

        pVertices[index].x = vecVertices[index].x;
        pVertices[index].y = vecVertices[index].y;
        pVertices[index].z = vecVertices[index].z;

        // Colours, too
        pVertices[index].r = 0.0f;
        pVertices[index].g = 1.0f;
        pVertices[index].b = 0.0f;
    }

    ::g_NumVerticesToDraw = numVertices;    // BEWARE!

    ::g_SizeOfVertexArrayInBytes = numVertices * sizeof(sVertex);


    // TODO: Copy from one place to the other..

    theFile.close();

    return true;
}

// NON-indexed model loader
bool loadPLY_XYZ_Format_Eldritch_Horror(std::string fileName)
{
    //2.24562 -0.864372 0.995778 
    struct sPlyVertexXYZ
    {
        float x, y, z;
    };

    //3 1068 1075 1076 
    struct sPlyTriangle
    {
        int numVertices;    // 3
        int v0, v1, v2;
    };

    std::vector<sPlyVertexXYZ> vecVertices;
    std::vector<sPlyTriangle> vecTriangles;

    std::ifstream theFile(fileName.c_str());
    if ( ! theFile.is_open() )
    {
        return false;
    }

    std::string curWord;

    std::cout << "Type in vertex to win!";
    while ( theFile >> curWord )
    {
        if ( curWord == "vertex" )
        {
            break;
        }
    }
    // element vertex 2903
    unsigned int numVertices = 0;
    theFile >> numVertices;


    while (theFile >> curWord)
    {
        if (curWord == "face")
        {
            break;
        }
    }
    // element face 5804
    unsigned int numTriangles = 0;
    theFile >> numTriangles;


    while (theFile >> curWord)
    {
        if (curWord == "end_header")
        {
            break;
        }
    }
    
    // Start to read the vertices...
    //  4.13317 2.17523 1.25932 
    //  4.23234 1.90308 0.534362
    for ( unsigned int count = 0; count < numVertices; count++ )
    {
        sPlyVertexXYZ curVert;

        theFile 
            >> curVert.x 
            >> curVert.y
            >> curVert.z;

        vecVertices.push_back(curVert);
    }

    //  3 250 251 210
    //  3 250 210 252
    for (unsigned int count = 0; count < numTriangles; count++)
    {
        sPlyTriangle curTri;

        theFile 
            >> curTri.numVertices       // 3
            >> curTri.v0 
            >> curTri.v1 
            >> curTri.v2;

        vecTriangles.push_back(curTri);
    }


    // At this point the model is loaded into the vecVertices and vecTriangles

    float scale = 0.1f;

 //   glm::vec3 skooth = glm::vec3(0.0f); // Set to all zeros
 //   skooth.x = 1.0f;
    glm::vec3 translate = glm::vec3(0.0f); // Set to all zeros
    translate.x = 3.1415f / 2.0f;

    float rotationZ = 0.0f;

//   for (unsigned int index = 0; index < vecVertices.size(); index++)
//   {
//        vecVertices[index].x *= scale;
//        vecVertices[index].y *= scale;
//        vecVertices[index].z *= scale;
//
///         vecVertices[index].x += translate.x;
///         vecVertices[index].y += translate.y;
///         vecVertices[index].z += translate.z;
//
//        // Also rotation: https://en.wikipedia.org/wiki/Rotation_matrix
//        vecVertices[index].x = vecVertices[index].x * glm::cos(rotationZ) - vecVertices[index].y * glm::sin(rotationZ);
//        vecVertices[index].y = vecVertices[index].x * glm::cos(rotationZ) + vecVertices[index].y * glm::sin(rotationZ);
///         vecVertices[index].z += translate.z;
//
//   }


    // Do some copying to the format that the shader wants...
//    struct sVertex {
//        float x, y, z;         // Positions
//        float r, g, b;          // Colours
//    };
//
//    sVertex vertices[3] = {
//        { -0.6f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f },   // B.R.
//        {  0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f },   // B.L.
//        {  0.0f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f }    // TOP
//    };

//    std::vector< sVertex> vecVerticesLikeTheyAreInTheShader;

//    sVertex* pVertices = NULL;  // 0, nulptr
    pVertices = new sVertex[numVertices];
    for ( unsigned int index = 0; index != numVertices; index++ )
    {
        // Copy from the ply type vector to the evil c array 
        //  that the code wants because it'

        pVertices[index].x = vecVertices[index].x;
        pVertices[index].y = vecVertices[index].y;
        pVertices[index].z = vecVertices[index].z;

        // Colours, too
        pVertices[index].r = 0.0f;
        pVertices[index].g = 1.0f;
        pVertices[index].b = 0.0f;
    }

    ::g_NumVerticesToDraw = numVertices;    // BEWARE!

    ::g_SizeOfVertexArrayInBytes = numVertices * sizeof(sVertex);


    // TODO: Copy from one place to the other..

    theFile.close();

    return true;
}


//static const char* vertex_shader_text =
//"#version 110\n"
//"uniform mat4 MVP;\n"
//"attribute vec3 vColour;\n"
//"attribute vec3 vPosition;\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPosition, 1.0);\n"
//"    color = vColour;\n"
//"}\n";
//
//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//     void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    const float CAMERA_SPEED = 0.1f;

    if ( mods == GLFW_MOD_SHIFT )       // 0001   // 0010  // 0100  // 0111
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

    if ( ( mods & GLFW_MOD_SHIFT ) == GLFW_MOD_SHIFT )
    {
        // Then shift (and maybe something else) is down

    }

    // Move the camera 
    if (key == GLFW_KEY_A )
    {
        // Go left
        ::g_cameraEye.x -= CAMERA_SPEED;
//        ::g_cameraTarget.x -= CAMERA_SPEED;
    }
    if (key == GLFW_KEY_D )
    {
        // Go right
        ::g_cameraEye.x += CAMERA_SPEED;
//        ::g_cameraTarget.x += CAMERA_SPEED;
    }

    if (key == GLFW_KEY_W )
    {
        // Go forward
        ::g_cameraEye.z -= CAMERA_SPEED;
//        ::g_cameraTarget.x -= CAMERA_SPEED;
    }
    if (key == GLFW_KEY_S )
    {
        // Go back
        ::g_cameraEye.z += CAMERA_SPEED;
//        ::g_cameraTarget.x += CAMERA_SPEED;
    }

    if (key == GLFW_KEY_Q )
    {
        // Go down
        ::g_cameraEye.y -= CAMERA_SPEED;
//        ::g_cameraTarget.x -= CAMERA_SPEED;
    }
    if (key == GLFW_KEY_E )
    {
        // Go up
        ::g_cameraEye.y += CAMERA_SPEED;
//        ::g_cameraTarget.x += CAMERA_SPEED;
    }
    return;
}

int main(void)
{
 //   MessageBox(NULL, L"I love OpenGL", L"Hey!", MB_OK);

//    int ax[3] = { 1, 2, 3 };    // Stack (at compile time)
//    int* pAX = new int[3];      // Heap (at run time)
    // Copy the data

    GLFWwindow* window;
    GLuint vertex_buffer;
//    GLuint vertex_shader;
//    GLuint fragment_shader;
    GLuint program = 0;
//    GLint mvp_location;

    GLint vpos_location;
    GLint vcol_location;

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

    if ( loadPLY_XYZ_Format("assets/models/cow.ply") )
//    if ( loadPLY_XYZ_confidence_intensity_format("assets/models/bun_zipper_res2.ply") )
    {
        std::cout << "Cow loaded OK." << std::endl;
    }
    else
    {
        std::cout << "ALL IS LOST!! FOREVER LOST!!!" << std::endl;
        // We're outta here
        return -1;
    }

//    // NOTE: OpenGL error checks have been omitted for brevity
//    glGenBuffers(1, &vertex_buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//    // Update this line here...
////    glBufferData(GL_ARRAY_BUFFER, 
////                 sizeof(vertices),      // How many bytes
////                 vertices,              // Pointer to the array
////                 GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, 
//                 ::g_SizeOfVertexArrayInBytes,      // How many bytes
//                 pVertices,                     // Pointer to the array
//                 GL_STATIC_DRAW);



//    cShaderManager* g_pTheShaderManager = NULL;   

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


 //   vertex_shader = glCreateShader(GL_VERTEX_SHADER);
 //   glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
 //   glCompileShader(vertex_shader);
 //
 //   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
 //   glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
 //   glCompileShader(fragment_shader);
 //
//   program = glCreateProgram();
//   glAttachShader(program, vertex_shader);
//   glAttachShader(program, fragment_shader);
//   glLinkProgram(program);

    //mvp_location = glGetUniformLocation(program, "MVP");

//    vpos_location = glGetAttribLocation(program, "vPosition");  // 17
//    vcol_location = glGetAttribLocation(program, "vColour");
//
////   struct sVertex
//   {
//       float x, y;         // Positions
//       float r, g, b;      // Colours
//   };
//
//   sVertex vertices[3] =
//   {
//       { -0.6f, -0.8f, 1.0f, 0.0f, 1.0f },   // B.R.
//       {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f },   // B.L.
//       {  0.0f,  3.0f, 0.0f, 0.0f, 1.0f }    // TOP
//   };
// 
//  
//
//
//    // attribute vec2 vPosition;
//    glEnableVertexAttribArray(vpos_location);
//    glVertexAttribPointer(vpos_location,        // Index: vPosition
//                          3,    // (now xyz)        // 2, 
//                          GL_FLOAT, 
//                          GL_FALSE,             // "normalized" or "clamped" -1 to 1
//                          sizeof(sVertex),      // Stride
//                          (void*)offsetof(sVertex, x) );            // Offset
////                          sizeof(vertices[0]), 
////                         (void*)0);            // Offset
//
////    int x = 656;
//    int* pX = &x;       // Pointer to x (type: integer pointer)
//    void* pOMG = &x;    // Type is a pointer to whatever
//    pOMG = key_callback;
//    std::cout << *(std::string*)(pOMG);
//    std::cout << (int*)(pOMG);
//
//    // attribute vec3 vColour;
//    glEnableVertexAttribArray(vcol_location);
//    glVertexAttribPointer(vcol_location,        // Index: vColour
//                          3, 
//                          GL_FLOAT, 
//                          GL_FALSE,
//                          sizeof(sVertex),      // Stride
//                          (void*)offsetof(sVertex, r) );    // Offset
////                          sizeof(vertices[0]), 
////                          (void*)(sizeof(float) * 2));
//

    // Load all the TYPES of models I can draw
    ::g_pMeshManager = new cVAOManager();

    sModelDrawInfo meshInfoCow;
    if ( ! ::g_pMeshManager->LoadModelIntoVAO( "assets/models/cow_xyz_rgba.ply", meshInfoCow, program ) )
    {
        std::cout << "ERROR: Didn't load the cow" << std::endl;
    }

    sModelDrawInfo carMeshInfo;
    if ( ! ::g_pMeshManager->LoadModelIntoVAO( "assets/models/de--lorean_xyz_rgba.ply", carMeshInfo, program ) )
    {
        std::cout << "ERROR: Didn't load the cow" << std::endl;
    }

    sModelDrawInfo teapotMeshInfo;
    if ( ! ::g_pMeshManager->LoadModelIntoVAO( "assets/models/Utah_Teapot_xyz_rgba.ply", teapotMeshInfo, program ) )
    {
        std::cout << "ERROR: Didn't load the cow" << std::endl;
    }


    // Load the models I'd like to draw in the scene
    cMeshObject* pCow = new cMeshObject();
    pCow->meshFileName = "assets/models/cow_xyz_rgba.ply";
    pCow->bIsWireFrame = false;
    pCow->position.x = -10.f;
    ::g_MeshesToDraw.push_back(pCow);

    cMeshObject* pCow2 = new cMeshObject();
    pCow2->meshFileName = "assets/models/cow_xyz_rgba.ply";
    pCow2->bIsWireFrame = false;
    pCow2->position.x = +10.0f;
    pCow2->scale = 0.5f;
    pCow->orientation.z = glm::radians(-45.0f);
    pCow2->bOverrideVertexModelColour = true;
    pCow2->colourRGB = glm::vec3(0.0f, 1.0f, 0.0f);
    ::g_MeshesToDraw.push_back(pCow2);

    cMeshObject* pCar = new cMeshObject();
    pCar->meshFileName = "assets/models/de--lorean_xyz_rgba.ply";
    pCar->orientation.x = glm::radians(-90.0f);
    pCar->position.z = 25.0f;
    pCar->bIsWireFrame = true;
    ::g_MeshesToDraw.push_back(pCar);

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
        glm::mat4 matProjection = glm::perspective(0.6f,
                             ratio,
                             0.1f,
                             1000.0f);



        glm::mat4 matView = glm::mat4(1.0f);

//        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -20.0f);
//        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        matView = glm::lookAt(::g_cameraEye,
                              ::g_cameraTarget,
                              upVector);

       //mat4x4_mul(mvp, p, m);
//        mvp = p * v * m;




        // Start drawing the scene
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
            

//            // Copying the mvp matrix to the shader
//            glm::mat4 mvp;
//            GLint mvp_location = glGetUniformLocation(program, "MVP");
//
////            glGetUniformLocation(program, "objectColourOverride")
////            glSetThisVariable("objectColourOverride", 2);
//
//            glUniformMatrix4fv(mvp_location,
//                               1, 
//                               GL_FALSE, 
//                               glm::value_ptr(mvp) );

    //        glDrawArrays(GL_TRIANGLES, 
    //                     0,         // Offset
    //                     3);        // How many we want to draw

    //         glDrawArrays(GL_TRIANGLES, 
    //                     0,         // Offset
    //                     ::g_NumVerticesToDraw);        // How many we want to draw


            // I removed MVP from the shader and am 
            //  passing the m, v, and p on thier own
            //uniform mat4 mProj;
            //uniform mat4 mView;
            //uniform mat4 mModel;

            GLint mProj_location = glGetUniformLocation(program, "mProj");
            GLint mView_location = glGetUniformLocation(program, "mView");
            GLint mModel_location = glGetUniformLocation(program, "mModel");

            glUniformMatrix4fv(mProj_location, 1, GL_FALSE, 
                               glm::value_ptr(matProjection) );

            glUniformMatrix4fv(mView_location, 1, GL_FALSE,
                               glm::value_ptr(matView) );

            glUniformMatrix4fv(mModel_location, 1, GL_FALSE,
                               glm::value_ptr(matModel) );


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
            << ::g_cameraEye.z;

//        glfwSetWindowTitle(window, "HEY!");
        glfwSetWindowTitle(window, ssWindowsTitle.str().c_str() );


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
