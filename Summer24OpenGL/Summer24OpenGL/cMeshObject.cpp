#include "cMeshObject.h"

cMeshObject::cMeshObject()
{
	this->position = glm::vec3(0.0f);
	this->orientation = glm::vec3(0.0f);
	this->scale = 1.0f;
	this->colourRGBA = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
};

