
#include "globalOpenGL.h"
#include "cMeshObject.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cVAOManager/cVAOManager.h"

extern cBasicTextureManager* g_TextureManager;
extern cVAOManager* g_pMeshManager;


void DrawMesh(cMeshObject* pCurrentMesh, GLuint shaderProgram)
{
                // Is it visible?
    if (!pCurrentMesh->bIsVisible)
    {
        // Skip it
        //continue;
        return;
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

//            // For normals, we could keep track of only the rotation
//            glm::mat4 matRotationOnly = matRotateX * matRotateY * matRotateZ;

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
    if (pCurrentMesh->bIsWireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
//            glPointSize(10);



    //GLint mProj_location = glGetUniformLocation(program, "mProj");
    //GLint mView_location = glGetUniformLocation(program, "mView");

    //glUniformMatrix4fv(mProj_location, 1, GL_FALSE,
    //                   glm::value_ptr(matProjection));

    //glUniformMatrix4fv(mView_location, 1, GL_FALSE,
    //                   glm::value_ptr(matView));

    GLint mModel_location = glGetUniformLocation(shaderProgram, "mModel");

    glUniformMatrix4fv(mModel_location, 1, GL_FALSE,
                       glm::value_ptr(matModel));

    // We need this for lighting
    // uniform mat4 mModel_InverseTranspose;
    GLint mModelIT_location = glGetUniformLocation(shaderProgram, "mModel_InverseTranspose");

    // Calculate the "inverse transpose of the model matrix"
    // Gets rid of any translation (movement) and scaling,
    //  leaving only the rotation transformation
    // (We use this for the normal)
    glm::mat4 matModelIT = glm::inverse(glm::transpose(matModel));

    glUniformMatrix4fv(mModelIT_location, 1, GL_FALSE,
                       glm::value_ptr(matModelIT));


    // Do I override the vertex colour
    GLint colourOverride_UL = glGetUniformLocation(shaderProgram, "colourOverride");
    GLint bUseOverrideColour_UL = glGetUniformLocation(shaderProgram, "bUseOverrideColour");

    if (pCurrentMesh->bOverrideVertexModelColour)
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

    // Copy over the specular value
    // uniform vec4 vertexSpecular;
    GLint vertexSpecular_UL = glGetUniformLocation(shaderProgram, "vertexSpecular");
    glUniform4f(vertexSpecular_UL,
                pCurrentMesh->specularHighlightColour.r,
                pCurrentMesh->specularHighlightColour.g,
                pCurrentMesh->specularHighlightColour.b,
                pCurrentMesh->shinniness);


    if (pCurrentMesh->alphaTransparency < 1.0f)
    {
        // it's transparent

        // Alpha transparency
        // We can enable or disable this any time (no performance hit)
        glEnable(GL_BLEND);
        // Takes some number form the 1st param
        // ...does something based on 2nd param
        // NOTE: This only needs to be set once
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set the transparency in the shader
        // uniform float alphaTransparency;

        GLint alphaTransparency_UL = glGetUniformLocation(shaderProgram, "alphaTransparency");
        // Use the transparency from the mesh
        glUniform1f(alphaTransparency_UL, pCurrentMesh->alphaTransparency);
    }
    else
    {
        // it's solid
        glDisable(GL_BLEND);
    }


    // Turn off lighting?
    // uniform bool bDoNotLight
    GLint bDoNotLight_UL = glGetUniformLocation(shaderProgram, "bDoNotLight");

    if (pCurrentMesh->bDoNotLight)
    {
//        glUniform1f(bDoNotLight_UL, 1.0f);
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_TRUE);
    }
    else
    {
//        glUniform1f(bDoNotLight_UL, 0.0f);
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_FALSE);

    }


    // Set up the last step in the texture.
    // Connect the texture in the GPU with the sampler in the shader
    // 

    // uniform bool bUseTextureAsColour;
    GLint bUseTextureAsColour_UL = glGetUniformLocation(shaderProgram, "bUseTextureAsColour");

    if (pCurrentMesh->bUseTextureAsColour)
    {
        glUniform1f(bUseTextureAsColour_UL, (GLfloat)GL_TRUE);
    }
    else
    {
        glUniform1f(bUseTextureAsColour_UL, (GLfloat)GL_FALSE);
    }


// ****************************************************************************

    //uniform sampler2D textures00;
    //uniform sampler2D textures01;
    //uniform sampler2D textures02;
    //uniform sampler2D textures03;
    //uniform vec4 textureRatios_0_to_3;	

    {//uniform sampler2D textures00;
        GLuint texture00_ID = ::g_TextureManager->getTextureIDFromName(pCurrentMesh->textureNames[0]);

        // Texture binding...
        GLuint texture00Unit = 0;			// Texture unit go from 0 to 79
        glActiveTexture(GL_TEXTURE0 + texture00Unit);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, texture00_ID);

        // uniform sampler2D texture01;
        GLint textures00_UL = glGetUniformLocation(shaderProgram, "textures00");
        // Set texture unit in the shader, too
        glUniform1i(textures00_UL, texture00Unit);
    //
    //    glBindTextureUnit( 21, dungeonTextID );	// OpenGL 4.5+ only
    }

    {   //uniform sampler2D textures01;
        GLuint texture01_ID = ::g_TextureManager->getTextureIDFromName(pCurrentMesh->textureNames[1]);
        GLuint texture01Unit = 1;			// Texture unit go from 0 to 79
        glActiveTexture(GL_TEXTURE0 + texture01Unit);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, texture01_ID);
        GLint textures01_UL = glGetUniformLocation(shaderProgram, "textures01");
        glUniform1i(textures01_UL, texture01Unit);
    }

    {   //uniform sampler2D textures02;
        GLuint texture02_ID = ::g_TextureManager->getTextureIDFromName(pCurrentMesh->textureNames[2]);
        GLuint texture02Unit = 2;			// Texture unit go from 0 to 79
        glActiveTexture(GL_TEXTURE0 + texture02Unit);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, texture02_ID);
        GLint textures02_UL = glGetUniformLocation(shaderProgram, "textures02");
        glUniform1i(textures02_UL, texture02Unit);
    }

    {   //uniform sampler2D textures03;
        GLuint texture03_ID = ::g_TextureManager->getTextureIDFromName(pCurrentMesh->textureNames[3]);
        GLuint texture03Unit = 3;			// Texture unit go from 0 to 79
        glActiveTexture(GL_TEXTURE0 + texture03Unit);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, texture03_ID);
        GLint textures03_UL = glGetUniformLocation(shaderProgram, "textures03");
        glUniform1i(textures03_UL, texture03Unit);
    }

    // Also the ratios
    //uniform vec4 textureRatios_0_to_3;	
    GLint textureRatios_0_to_3_UL = glGetUniformLocation(shaderProgram, "textureRatios_0_to_3");
    glUniform4f(textureRatios_0_to_3_UL,
                pCurrentMesh->textureBlendRatio[0],     // x
                pCurrentMesh->textureBlendRatio[1],     // y
                pCurrentMesh->textureBlendRatio[2],     // z
                pCurrentMesh->textureBlendRatio[3]);    // w



// ****************************************************************************

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

    return;
}
