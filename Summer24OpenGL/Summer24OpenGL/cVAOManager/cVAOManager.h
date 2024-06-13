#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>

// The vertex structure, as it is in the SHADER (on the GPU)
// This is also called the 'vertex layout'. 
// This means that if the shader vertex layout changed, this
//  vertex structure will also need to change (usually). 
//struct sVert_OG
//{
//	float x, y, z;		// added "z"
//	float r, g, b;
//};
//
//struct sVert_xyzw_RGBA
//{
//	float x, y, z, w;		// added "z"
//	float r, g, b, a;
//};

// Adding the normals because file now has normals:
//ply
//element vertex 2903
//property float x
//property float y
//property float z
//property float nx			<--
//property float ny
//property float nz
//property uchar red
//property uchar green
//property uchar blue
//property uchar alpha
//struct sVert_xyzw_n_RGBA
//{
//	float x, y, z, w;		//  position (ignoring the w)
//	float nx, ny, nz, nw;	//	normal (ignoring the w)
//	float r, g, b, a;
//};

//	property float x
//	property float y
//	property float z
//	property float nx
//	property float ny
//	property float nz
//	property uchar red
//	property uchar green
//	property uchar blue
//	property uchar alpha
//	property float texture_u
//	property float texture_v		***
struct sVert_xyzw_n_RGBA_uv
{
	float x, y, z, w;		//  position (ignoring the w)
	float nx, ny, nz, nw;	//	normal (ignoring the w)
	float r, g, b, a;
	float u, v;				// Texture coords
};

// This represents a single mesh (3D object) structure in 'indexed' format. 
// Its layout is set up to match how the GPU sees the mesh, rather
//  than how the file was, etc. 
// It's also storing the infomation that we need to tell the GPU
//  which model we want to draw. 
struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
//	sVert_xyzw_RGBA* pVertices;	//  = 0;
//	sVert_xyzw_n_RGBA* pVertices;	//  = 0;
	sVert_xyzw_n_RGBA_uv* pVertices;
	// The index buffer (CPU side)
	unsigned int* pIndices;


};


class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID = 0,
						  bool bGenerateSphericalUVs = false);


	// Here's an option to load various models using boolean flags:
	bool LoadModelIntoVAO(std::string fileName,
						  sModelDrawInfo& drawInfo,
						  unsigned int shaderProgramID,
						  bool bModelHasColours,
						  bool bModelHasNormals,
						  bool bUseTextureCoords);

	// ...or you could have specific functions to load the files:
	bool LoadModelIntoVAO_XYZ_ONLY(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);



private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	// Loads the ply model file into a temporary array
	bool m_LoadTheModel( std::string fileName, 
						 sModelDrawInfo &drawInfo,
						 bool bGenerateSphericalUVs);

	std::string m_lastErrorString;
	void m_AppendTextToLastError(std::string text, bool addNewLineBefore = true);

	// These are needed for the call below
	enum enumTEXCOORDBIAS
	{
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};
	void m_GenTextureCoordsSpherical(sModelDrawInfo& meshInfo,
									 enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias,
									 bool basedOnNormals, float scale, bool fast);
};

#endif	// _cVAOManager_HG_
