#include "cMeshObject.h"

cMeshObject::cMeshObject()
{
	this->position = glm::vec3(0.0f);
	this->orientation = glm::vec3(0.0f);
	this->scale = 1.0f;
	this->colourRGB = glm::vec3( 1.0f, 1.0f, 1.0f );
	this->bOverrideVertexModelColour = false;

	this->alphaTransparency = 1.0f;

	this->bDoNotLight = false;

	this->bUseTextureAsColour = true;

	// 
	this->textureBlendRatio[0] = 0.0f;
	this->textureBlendRatio[1] = 0.0f;
	this->textureBlendRatio[2] = 0.0f;
	this->textureBlendRatio[3] = 0.0f;
	this->textureBlendRatio[4] = 0.0f;
	this->textureBlendRatio[5] = 0.0f;
	this->textureBlendRatio[6] = 0.0f;
	this->textureBlendRatio[7] = 0.0f;

	this->bIsWireFrame = false;	// true;
	this->bIsVisible = true;

	this->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	this->shinniness = 1.0f;

	this->m_uniqueID = cMeshObject::m_CurrentID;
	cMeshObject::m_CurrentID++;
};


//static 
unsigned int cMeshObject::m_CurrentID = 1000;
