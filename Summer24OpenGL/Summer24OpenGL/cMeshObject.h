#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>


// This represents a thing we are drawing on the screen.
// 
// CRC: Class, Responsibility, Collaboration
// A "class" should be a noun or a thing (and just one thing)
class cMeshObject
{
public:
	cMeshObject();
	// Mesh file name
	// Position, orientation, scale, colour
	std::string meshFileName;	// File of the model
	std::string uniqueName;		// Some identifier for THIS model
	glm::vec3 position;
	glm::vec3 orientation;	// Euler axes
//	glm::vec3 scaleXYZ;
	float scale;			// Uniform scale 
	glm::vec4 colourRGBA;

	// For later: child objects
	std::vector< cMeshObject* > vec_pChildObjects;
};

