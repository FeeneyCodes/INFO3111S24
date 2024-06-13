#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>


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

	// If true, then use the colourRGB
	bool bOverrideVertexModelColour;
	glm::vec3 colourRGB;
	// This is really the 4th parameter being drawn on the framebuffer (screen)
	float alphaTransparency; 

	// Specular (shininess)
	// uniform vec4 vertexSpecular;		RGB = colour
	//									W (4th value) = 1 to 1,000,000?
	glm::vec3 specularHighlightColour;	// RGB colour of the highlight
	float shinniness;					// How shiny this is 1.0, 10, 100, 1000
	// Called "specular power"

	bool bIsWireFrame;
	bool bIsVisible;

	bool bDoNotLight;

	// We'll have 8 textures eventually
	bool bUseTextureAsColour;
	static const unsigned int NUM_TEXTURES = 8;
	std::string textureNames[NUM_TEXTURES];
	float textureBlendRatio[NUM_TEXTURES];		// From 0 to 1.0f


	// This is some name we can give it
	std::string friendlyName;

	// For later: child objects
	std::vector< cMeshObject* > vec_pChildObjects;

	unsigned int getUniqueID(void)
	{
		return this->m_uniqueID;
	}
private:
	unsigned int m_uniqueID = 0;
	// Note that static you can't set the value here
	static unsigned int m_CurrentID;// = 1000;
};

