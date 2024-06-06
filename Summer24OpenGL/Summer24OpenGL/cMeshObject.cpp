#include "cMeshObject.h"

cMeshObject::cMeshObject()
{
	this->position = glm::vec3(0.0f);
	this->orientation = glm::vec3(0.0f);
	this->scale = 1.0f;
	this->colourRGB = glm::vec3( 1.0f, 1.0f, 1.0f );
	this->bOverrideVertexModelColour = false;

	this->bIsWireFrame = false;	// true;
	this->bIsVisible = true;

	this->specularHighlightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	this->shinniness = 1.0f;

	this->m_uniqueID = cMeshObject::m_CurrentID;
	cMeshObject::m_CurrentID++;
};


//static 
unsigned int cMeshObject::m_CurrentID = 1000;
