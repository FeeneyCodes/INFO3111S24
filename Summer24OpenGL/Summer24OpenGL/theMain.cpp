#include <glad/glad.h>
#include <GLFW/glfw3.h>
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



struct sVertex
{
    float x, y, z;         // Positions
    float r, g, b;          // Colours
};

sVertex vertices[3] =
{
    { -0.6f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f },   // B.R.
    {  0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f },   // B.L.
    {  0.0f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f }    // TOP
};

bool loadPLY_XYZ_Format(std::string fileName)
{
    std::ifstream theFile(fileName.c_str());

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
    for ( unsigned int count = 0; count < numVertices; count++ )
    {
        //
    }

    //  4.13317 2.17523 1.25932 
    //  4.23234 1.90308 0.534362
    //  3 250 251 210
    //  3 250 210 252

    for (unsigned int count = 0; count < numTriangles; count++)
    {
        //
    }



    theFile.close();

    return true;
}


static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vColour;\n"
"attribute vec3 vPosition;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPosition, 1.0);\n"
"    color = vColour;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
 //   MessageBox(NULL, L"I love OpenGL", L"Hey!", MB_OK);


    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location;

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
    {
        std::cout << "Cow loaded OK." << std::endl;
    }
    else
    {
        std::cout << "ALL IS LOST!! FOREVER LOST!!!" << std::endl;
        // We're outta here
        return -1;
    }

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    // Update this line here...
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);


    mvp_location = glGetUniformLocation(program, "MVP");

    vpos_location = glGetAttribLocation(program, "vPosition");  // 17
    vcol_location = glGetAttribLocation(program, "vColour");

//   struct sVertex
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


    // attribute vec2 vPosition;
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location,        // Index: vPosition
                          3,    // (now xyz)        // 2, 
                          GL_FLOAT, 
                          GL_FALSE,             // "normalized" or "clamped" -1 to 1
                          sizeof(sVertex),      // Stride
                          (void*)offsetof(sVertex, x) );            // Offset
//                          sizeof(vertices[0]), 
//                         (void*)0);            // Offset

//    int x = 656;
//    int* pX = &x;       // Pointer to x (type: integer pointer)
//    void* pOMG = &x;    // Type is a pointer to whatever
//    pOMG = key_callback;
//    std::cout << *(std::string*)(pOMG);
//    std::cout << (int*)(pOMG);

    // attribute vec3 vColour;
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location,        // Index: vColour
                          3, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(sVertex),      // Stride
                          (void*)offsetof(sVertex, r) );    // Offset
//                          sizeof(vertices[0]), 
//                          (void*)(sizeof(float) * 2));

   
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
 //       mat4x4 m, p, mvp;
        glm::mat4 m, p, v, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        //         mat4x4_identity(m);
        m = glm::mat4(1.0f);

        //mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
                                        (float)glfwGetTime(),
                                        glm::vec3(0.0f, 0.0, 1.0f));

//        m = m * rotateZ;

        //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        p = glm::perspective(0.6f,
                             ratio,
                             0.1f,
                             1000.0f);

        v = glm::mat4(1.0f);

        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        v = glm::lookAt(cameraEye,
                        cameraTarget,
                        upVector);

       //mat4x4_mul(mvp, p, m);
        mvp = p * v * m;


        glUseProgram(program);


        //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
