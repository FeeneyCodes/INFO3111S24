#include "cVAOManager.h"

#include "../globalOpenGL.h"

#include <fstream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>		// normalize()

#include <vector>

#include <sstream>

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;	// or NULL
	this->pIndices = 0;		// or NULL

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	glm::vec3 maxValues = glm::vec3(0.0f);
	glm::vec3 minValues = glm::vec3(0.0f);

//	scale = 5.0/maxExtent;		-> 5x5x5
	float maxExtent = 0.0f;

	return;
}




// Generates texture coordinates based on a shperical projection from the 
// origin (0,0,0) location of the model. 
// NOTE: Will overwrite any existing texture coordinates 
// Inspired by: http://www.mvps.org/directx/articles/spheremap.htm
// 
//	basedOnNormals = true --> uses normals for calculation, so good for reflections
//	basedOnNormals = false --> uses model coordinates, so more 'shrink wrapped'
//  fast -> approximate or not (not uses sin() )
//  scale -> normally 1.0, but can be the limits of the texture coord
//
// The 'bias' is what coordinates are used for the generation. 
// For example, if POSITIVE_X is for u and POSITIVE_Y is for v, then:
//	u coords: based on -x to +x
//  v coords: based on -y to +y
//
void cVAOManager::m_GenTextureCoordsSpherical(sModelDrawInfo& meshInfo,
							   enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias,
							   bool basedOnNormals, float scale, bool fast)
{
	// determine the 'remaining' coordinate...
	bool xUsed = false;
	bool yUsed = false;
	bool zUsed = false;
	if (uBias == POSITIVE_X || vBias == POSITIVE_X)	xUsed = true;
	if (uBias == POSITIVE_Y || vBias == POSITIVE_Y)	yUsed = true;
	if (uBias == POSITIVE_Z || vBias == POSITIVE_Z)	yUsed = true;

	for (unsigned int vertIndex = 0; vertIndex != meshInfo.numberOfVertices; vertIndex++)
	{
		sVert_xyzw_n_RGBA_uv curVertex = meshInfo.pVertices[vertIndex];

		glm::vec3 xyz = glm::vec3(curVertex.x,
								  curVertex.y,
								  curVertex.z);

		if (basedOnNormals)
		{
			if (uBias == POSITIVE_X)		xyz.x = curVertex.nx;
			else if (uBias == POSITIVE_Y)	xyz.x = curVertex.ny;
			else if (uBias == POSITIVE_Z)	xyz.x = curVertex.nz;

			if (vBias == POSITIVE_X)		xyz.y = curVertex.nx;
			else if (vBias == POSITIVE_Y)	xyz.y = curVertex.ny;
			else if (vBias == POSITIVE_Z)	xyz.y = curVertex.nz;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = curVertex.nx;
			if (!yUsed)	xyz.z = curVertex.ny;
			if (!zUsed)	xyz.z = curVertex.nz;
		}
		else
		{
			if (uBias == POSITIVE_X)		xyz.x = curVertex.x;
			else if (uBias == POSITIVE_Y)	xyz.x = curVertex.y;
			else if (uBias == POSITIVE_Z)	xyz.x = curVertex.z;

			if (vBias == POSITIVE_X)		xyz.y = curVertex.x;
			else if (vBias == POSITIVE_Y)	xyz.y = curVertex.y;
			else if (vBias == POSITIVE_Z)	xyz.y = curVertex.z;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = curVertex.x;
			if (!yUsed)	xyz.z = curVertex.y;
			if (!zUsed)	xyz.z = curVertex.z;
		}

		xyz = glm::normalize(xyz);

		if (fast)
		{
			meshInfo.pVertices[vertIndex].u = ((xyz.x / 2.0f) + 0.5f) * scale;
			meshInfo.pVertices[vertIndex].v = ((xyz.y / 2.0f) + 0.5f) * scale;
		}
		else
		{
			const float PI = 3.14157f;
			meshInfo.pVertices[vertIndex].u = ((asin(xyz.x) / PI) + 0.5f) * scale;
			meshInfo.pVertices[vertIndex].v = ((asin(xyz.y) / PI) + 0.5f) * scale;
		}
	}
	return;
}


bool cVAOManager::LoadModelIntoVAO(
		std::string fileName, 
		sModelDrawInfo &drawInfo,
	    unsigned int shaderProgramID,
		bool bGenerateSphericalUVs /*=false*/)

{
	// Load the model from file
	// (We do this here, since if we can't load it, there's 
	//	no point in doing anything else, right?)

	drawInfo.meshName = fileName;

	if ( ! this->m_LoadTheModel( fileName, drawInfo, bGenerateSphericalUVs) )
	{
		this->m_AppendTextToLastError( "Didn't load model", true );
		return false;
	}


	//// Move model
	//for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
	//{
	//	drawInfo.pVertices[index].x *= 0.1f;
	//	drawInfo.pVertices[index].y *= 0.1f;
	//	drawInfo.pVertices[index].z *= 0.1f;

	//	//drawInfo.pVertices[index].r = (rand() % 255)/255.0f;
	//	//drawInfo.pVertices[index].g = (rand() % 255) / 255.0f;
	//	//drawInfo.pVertices[index].b = (rand() % 255) / 255.0f;

	//	//// z goes from -25 to 40
	//	//float rangeX = 40.0f - (-25.0f);

	//	//// Where's the z in this range
	//	//float zColour = drawInfo.pVertices[index].z / rangeX;
	//	//drawInfo.pVertices[index].r = zColour;
	//	//drawInfo.pVertices[index].g = zColour;
	//	//drawInfo.pVertices[index].b = zColour;
	//}



	// 
	// Model is loaded and the vertices and indices are in the drawInfo struct
	// 

	// Create a VAO (Vertex Array Object), which will 
	//	keep track of all the 'state' needed to draw 
	//	from this buffer...

	// Ask OpenGL for a new buffer ID...
	glGenVertexArrays( 1, &(drawInfo.VAO_ID) );
	// "Bind" this buffer:
	// - aka "make this the 'current' VAO buffer
	glBindVertexArray(drawInfo.VAO_ID);

	// Now ANY state that is related to vertex or index buffer
	//	and vertex attribute layout, is stored in the 'state' 
	//	of the VAO... 


	// NOTE: OpenGL error checks have been omitted for brevity
//	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &(drawInfo.VertexBufferID) );

//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
	// sVert vertices[3]
	glBufferData( GL_ARRAY_BUFFER, 
//				  sizeof(sVert) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
//				  sizeof(sVert_xyzw_RGBA) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
//				  sizeof(sVert_xyzw_n_RGBA) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
				  sizeof(sVert_xyzw_n_RGBA_uv) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
				  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
				  GL_STATIC_DRAW );


	// Copy the index buffer into the video card, too
	// Create an index buffer.
	glGenBuffers( 1, &(drawInfo.IndexBufferID) );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

	glBufferData( GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
	              sizeof( unsigned int ) * drawInfo.numberOfIndices, 
	              (GLvoid*) drawInfo.pIndices,
                  GL_STATIC_DRAW );

	// Set the vertex attributes.

	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	// program
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");		// program;
	// Normals, too
	GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");	
	// Texture coordinates, too			// in vec2 vUV;
	GLint vUV_location = glGetAttribLocation(shaderProgramID, "vUV");

	// Set the vertex attributes for this shader
	glEnableVertexAttribArray(vpos_location);	// vPosition
	//glVertexAttribPointer(vpos_location, 4,	// vPosition
	//					  GL_FLOAT, GL_FALSE,
	//					  sizeof(sVert_xyzw_RGBA),			// sizeof(float) * 32,		// Stride
	//					  (void*)offsetof(sVert_xyzw_RGBA, x));   //(void*)0);				// Offset
	glVertexAttribPointer(vpos_location, 4,	// vPosition
						  GL_FLOAT, GL_FALSE,
						  sizeof(sVert_xyzw_n_RGBA_uv),			// sizeof(float) * 32,		// Stride
						  (void*)offsetof(sVert_xyzw_n_RGBA_uv, x));   //(void*)0);				// Offset

	glEnableVertexAttribArray(vcol_location);	// vColour
	//glVertexAttribPointer( vcol_location, 4,	// vColour
	//					   GL_FLOAT, GL_FALSE,
	//					   sizeof(sVert_xyzw_RGBA),		// sizeof(float) * 6,
	//					   (void*)offsetof(sVert_xyzw_RGBA, r));		// ( void* )( sizeof(float) * 4 ));
	glVertexAttribPointer( vcol_location, 4,	// vColour
						   GL_FLOAT, GL_FALSE,
						   sizeof(sVert_xyzw_n_RGBA_uv),		// sizeof(float) * 6,
						   (void*)offsetof(sVert_xyzw_n_RGBA_uv, r));		// ( void* )( sizeof(float) * 4 ));

	// Also the normals...
	glEnableVertexAttribArray(vnorm_location);	// vNormal
	glVertexAttribPointer(vnorm_location, 4,	// vUV
						   GL_FLOAT, GL_FALSE,
						   sizeof(sVert_xyzw_n_RGBA_uv),
						   (void*)offsetof(sVert_xyzw_n_RGBA_uv, nx));


	// Texture coordinates, too			// in vec2 vUV;
	//GLint vUV_location = glGetAttribLocation(shaderProgramID, "vUV");
	glEnableVertexAttribArray(vUV_location);	// vUV
	glVertexAttribPointer(vUV_location, 2,	// vUV
						   GL_FLOAT, GL_FALSE,
						   sizeof(sVert_xyzw_n_RGBA_uv),
						   (void*)offsetof(sVert_xyzw_n_RGBA_uv, u));



	// Now that all the parts are set up, set the VAO to zero
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vnorm_location);
	glDisableVertexAttribArray(vUV_location);	


	// Store the draw information into the map
	this->m_map_ModelName_to_VAOID[ drawInfo.meshName ] = drawInfo;


	return true;
}


// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(
		std::string filename,
		sModelDrawInfo &drawInfo) 
{
	std::map< std::string /*model name*/,
			sModelDrawInfo /* info needed to draw*/ >::iterator 
		itDrawInfo = this->m_map_ModelName_to_VAOID.find( filename );

	// Find it? 
	if ( itDrawInfo == this->m_map_ModelName_to_VAOID.end() )
	{
		// Nope
		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	drawInfo = itDrawInfo->second;
	return true;
}


bool cVAOManager::m_LoadTheModel(std::string fileName,
								 sModelDrawInfo &drawInfo,
								 bool bGenerateSphericalUVs)
{
	// Open the file. 
	// Read until we hit the word "vertex"
	// Read until we hit the word "face"
	// Read until we hit the word "end_header"

	std::ifstream thePlyFile( fileName.c_str() );
	if ( ! thePlyFile.is_open() )
	{	// Something is wrong...
		std::stringstream ssError;
		ssError << "Can't open >" << fileName << "< file." << std::endl;
		this->m_AppendTextToLastError(ssError.str());
		return false;
	}

	std::string temp; 
	while ( thePlyFile >> temp )
	{
		if ( temp == "vertex" ) 
		{
			break;
		}
	}; 
	// read the number of vertices...
	thePlyFile >> drawInfo.numberOfVertices;	//::g_NumberOfVertices;

	while ( thePlyFile >> temp )
	{
		if ( temp == "face" ) 
		{
			break;
		}
	}; 
	// read the number of triangles...
	thePlyFile >> drawInfo.numberOfTriangles;		// ::g_NumberOfTriangles;


	while ( thePlyFile >> temp )
	{
		if ( temp == "end_header" ) 
		{
			break;
		}
	}; 

	// And now, we start reading vertices... Hazzah!
	
	// This is set up to match the ply (3d model) file. 
	// NOT the shader. 
	struct sVertPly
	{
		glm::vec3 pos;
		// Vertex normals
		glm::vec3 normal;
		glm::vec4 colour;
		// 
		glm::vec2 texture_UV;
	};

	std::vector<sVertPly> vecTempPlyVerts;

	sVertPly tempVert;

	//property float x
	//property float y
	//property float z
	//property uchar red
	//property uchar green
	//property uchar blue
	//property uchar alpha

	// Load the vertices...
	for ( unsigned int index = 0; index != drawInfo.numberOfVertices; // ::g_NumberOfVertices; 
		  index++ )
	{
//                                           _          
//   __  __     _   _       __ _  _ __    __| |  ____ _ 
//   \ \/ /    | | | |     / _` || '_ \  / _` | |_  /(_)
//    >  <  _  | |_| | _  | (_| || | | || (_| |  / /  _ 
//   /_/\_\( )  \__, |( )  \__,_||_| |_| \__,_| /___|(_)
//         |/   |___/ |/                                

		thePlyFile >> tempVert.pos.x >> tempVert.pos.y >> tempVert.pos.z;


		//	bool bModelHasColours,
		//	bool bModelHasNormals,
		//	bool bUseTextureCoords);


//   __     __        _              _   _                            _       
//   \ \   / /__ _ __| |_ _____  __ | \ | | ___  _ __ _ __ ___   __ _| |___ _ 
//    \ \ / / _ \ '__| __/ _ \ \/ / |  \| |/ _ \| '__| '_ ` _ \ / _` | / __(_)
//     \ V /  __/ |  | ||  __/>  <  | |\  | (_) | |  | | | | | | (_| | \__ \_ 
//      \_/ \___|_|   \__\___/_/\_\ |_| \_|\___/|_|  |_| |_| |_|\__,_|_|___(_)
//                                                                            
//	property float nx
//	property float ny
//	property float nz
		thePlyFile >> tempVert.normal.x >> tempVert.normal.y >> tempVert.normal.z;

//     ____        _                             
//    / ___| ___  | |  ___   _   _  _ __  ___  _ 
//   | |    / _ \ | | / _ \ | | | || '__|/ __|(_)
//   | |___| (_) || || (_) || |_| || |   \__ \ _ 
//    \____|\___/ |_| \___/  \__,_||_|   |___/(_)
//                                               
		thePlyFile >> tempVert.colour.x >> tempVert.colour.y
			       >> tempVert.colour.z >> tempVert.colour.w; 

		// Scale the colour from 0 to 1, instead of 0 to 255
		// (the colours in the ply are "uchar" (unsigned char, aka unsinged byte) not float.
		// They range form 0 to 255 (like HTML colours), whereas we want them to be 0.0f to 1.0f
		tempVert.colour.x /= 255.0f;
		tempVert.colour.y /= 255.0f;
		tempVert.colour.z /= 255.0f;

		// property float texture_u
		// property float texture_v
//		float discard = 0.0f;
//		thePlyFile >> discard >> discard;
		thePlyFile
			>> tempVert.texture_UV.x
			>> tempVert.texture_UV.y;
			

		// Add too... what? 
		vecTempPlyVerts.push_back(tempVert);
	}






	// Create a local vertex array
	// Note: The format the file (ply) is DIFFERENT from this array:
	// - sVertPly was made to match the file format
	// - sVert was made to match the shader vertex attrib format

//	drawInfo.pVertices = new sVert[drawInfo.numberOfVertices];
//	drawInfo.pVertices = new sVert_xyzw_RGBA[drawInfo.numberOfVertices];
//	drawInfo.pVertices = new sVert_xyzw_n_RGBA[drawInfo.numberOfVertices];
	drawInfo.pVertices = new sVert_xyzw_n_RGBA_uv[drawInfo.numberOfVertices];

	// Optional clear array to zero 
	//memset( drawInfo.pVertices, 0, sizeof(sVert) * drawInfo.numberOfVertices);

	for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
	{
		drawInfo.pVertices[index].x = vecTempPlyVerts[index].pos.x;
		drawInfo.pVertices[index].y = vecTempPlyVerts[index].pos.y;
		drawInfo.pVertices[index].z = vecTempPlyVerts[index].pos.z;
		//
		drawInfo.pVertices[index].w = 1.0f;	// If in doubt, set 4th value to 1

		drawInfo.pVertices[index].r = vecTempPlyVerts[index].colour.r;
		drawInfo.pVertices[index].g = vecTempPlyVerts[index].colour.g;
		drawInfo.pVertices[index].b = vecTempPlyVerts[index].colour.b;
		drawInfo.pVertices[index].a = vecTempPlyVerts[index].colour.a;

		// Also normals:
		drawInfo.pVertices[index].nx = vecTempPlyVerts[index].normal.x;
		drawInfo.pVertices[index].ny = vecTempPlyVerts[index].normal.y;
		drawInfo.pVertices[index].nz = vecTempPlyVerts[index].normal.z;
		drawInfo.pVertices[index].nw = 1.0f;	// If in doubt, set 4th to 1

		// Also texture coordinates
		drawInfo.pVertices[index].u = vecTempPlyVerts[index].texture_UV.x;
		drawInfo.pVertices[index].v = vecTempPlyVerts[index].texture_UV.y;


	}// for ( unsigned int index...

	// Are we generating UVs?
	if (bGenerateSphericalUVs)
	{
		this->m_GenTextureCoordsSpherical(drawInfo,
										  POSITIVE_X, POSITIVE_Y,
										  false,		// Use normals
										  1.0f,			// 0 to 1
										  false);		// false
	}

	struct sTriPly
	{
		unsigned int vindex[3];		// the 3 indices
		sVertPly verts[3];			// The actual vertices
	};

	std::vector<sTriPly> vecTempPlyTriangles;
	sTriPly tempTriangle;
	unsigned int discard = 0;
	for ( unsigned int index = 0; index != drawInfo.numberOfTriangles;	// ::g_NumberOfTriangles; 
		  index++ )
	{
		// 3 5622 5601 5640
		thePlyFile >> discard						// the "3" at the start
			       >> tempTriangle.vindex[0]
			       >> tempTriangle.vindex[1]
			       >> tempTriangle.vindex[2];

		//// Look up the vertex that matches the triangle index values.
		//tempTriangle.verts[0] = vecTempPlyVerts[ tempTriangle.vindex[0] ];
		//tempTriangle.verts[1] = vecTempPlyVerts[ tempTriangle.vindex[1] ];
		//tempTriangle.verts[2] = vecTempPlyVerts[ tempTriangle.vindex[2] ];

		vecTempPlyTriangles.push_back( tempTriangle );
	}//for ( unsigned int index...

	
	// NOW, we need to put them into the vertex array buffer that 
	//	will be passed to OpenGL. Why? 
	// Because we called glDrawArrays(), that's why. 

//	::g_NumberOfVertsToDraw = ::g_NumberOfTriangles * 3;	// 3 because "triangles"

	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;

	// sVert* pVertices = 0;
//	pVertices = new sVert[::g_NumberOfVertsToDraw];

	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

	// Optional clear array to zero 
	//memset( drawInfo.pIndices, 0, sizeof(unsigned int) * drawInfo.numberOfIndices);

	// Allocate on the HEAP, so infinite size... 
	// delete pVertices			/// error!
	// delete [] pVertices		/// correct!!
	unsigned int indexBufferIndex = 0;
	for ( unsigned int triIndex = 0; 
		  triIndex != drawInfo.numberOfTriangles;		// ::g_NumberOfTriangles; 
		  triIndex++, indexBufferIndex += 3 )
	{
		sTriPly& curTri = vecTempPlyTriangles[triIndex];

		// This is taking an array of triangles vertex indices 
		// (which is a 2D array of indices, really) and converting
		//  it into a 1D array of indices...)
		drawInfo.pIndices[indexBufferIndex + 0] = curTri.vindex[0];
		drawInfo.pIndices[indexBufferIndex + 1] = curTri.vindex[1];
		drawInfo.pIndices[indexBufferIndex + 2] = curTri.vindex[2];

		//pVertices[ vertIndex + 0 ].x = curTri.verts[0].pos.x;
		//pVertices[ vertIndex + 0 ].y = curTri.verts[0].pos.y;
		//pVertices[ vertIndex + 0 ].z = curTri.verts[0].pos.z;
		//pVertices[ vertIndex + 0 ].r = curTri.verts[0].colour.x;
		//pVertices[ vertIndex + 0 ].g = curTri.verts[0].colour.y;
		//pVertices[ vertIndex + 0 ].b = curTri.verts[0].colour.z;
//
		//pVertices[ vertIndex + 1 ].x = curTri.verts[1].pos.x;
		//pVertices[ vertIndex + 1 ].y = curTri.verts[1].pos.y;
		//pVertices[ vertIndex + 1 ].z = curTri.verts[1].pos.z;
		//pVertices[ vertIndex + 1 ].r = curTri.verts[1].colour.x;
		//pVertices[ vertIndex + 1 ].g = curTri.verts[1].colour.y;
		//pVertices[ vertIndex + 1 ].b = curTri.verts[1].colour.z;
//
		//pVertices[ vertIndex + 2 ].x = curTri.verts[2].pos.x;
		//pVertices[ vertIndex + 2 ].y = curTri.verts[2].pos.y;
		//pVertices[ vertIndex + 2 ].z = curTri.verts[2].pos.z;
		//pVertices[ vertIndex + 2 ].r = curTri.verts[2].colour.x;
		//pVertices[ vertIndex + 2 ].g = curTri.verts[2].colour.y;
		//pVertices[ vertIndex + 2 ].b = curTri.verts[2].colour.z;
//
	}// for ( unsigned int triIndex = 0...

	return true;
}


std::string cVAOManager::getLastError(bool bAndClear /*=true*/)
{
	std::string theLastError = this->m_lastErrorString;

	if ( bAndClear )
	{
		this->m_lastErrorString = "";
	}

	return theLastError;
}

void cVAOManager::m_AppendTextToLastError(std::string text, bool addNewLineBefore /*=true*/)
{
	std::stringstream ssError;
	ssError << this->m_lastErrorString;

	if ( addNewLineBefore )
	{
		ssError << std::endl;
	}

	ssError << text;

	this->m_lastErrorString = ssError.str();

	return;

}
