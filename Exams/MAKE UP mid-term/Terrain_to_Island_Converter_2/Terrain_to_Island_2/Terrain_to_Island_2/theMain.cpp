#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/vec3.hpp"

struct sVertex
{
	sVertex()
	{
		this->pos = glm::vec3(0.0f);
		this->rgba = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->norm = glm::vec3(0.0f);
		this->UV = glm::vec2(0.0f);
	}
	glm::vec3 pos;
	glm::vec4 rgba;
	glm::vec3 norm;
	glm::vec2 UV;
};


bool ReadToToken( std::string token, std::ifstream &theFile )
{
	std::string curToken;
	while ( theFile >> curToken )
	{
		if ( curToken == token )
		{
			return true;
		}
	};
	return false;
}

float getMax( float A, float B )
{
	if ( A > B ) 
	{
		return A;
	}
	return B;
}

float getMin( float A, float B )
{
	if ( A < B )
	{
		return A;
	}
	return B;
}

struct sSphere
{
	glm::vec3 c;
	float r;
};
typedef glm::vec3 Point;
typedef glm::vec3 Vector;

bool IntersectRaySphere(Point p, Vector d, sSphere s, float &t, Point &q);

struct sLimits
{
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;
	glm::vec3 extentXYZ;
	glm::vec3 centreXYZ;
	float maxExtent;
};

void CalcExtents( std::vector<sVertex> &vecVerts, sLimits &limits );


glm::vec3 HTML_to_RGB(glm::vec3 HTML_Colour);

bool areAdjacentTrianglesFlat(std::vector<sVertex>& vecVerts, std::vector<glm::ivec3> &vecTris);

// Will remove any triangles associated with this vertex from the vecTriSource
//	and place them on the vecTrisRemoved.
unsigned int removeAssociatedTriangles(unsigned int vertIndex,
									   std::vector<glm::ivec3>& vecTriSource,
									   std::vector<glm::ivec3>& vecTrisRemoved);

void simplifyWaterMesh(std::vector<sVertex>& vecVerts, std::vector<glm::ivec3>& vecTris);

void appendTriangles( std::vector<glm::ivec3>& vecTris, std::vector<glm::ivec3>& vecTrisToAdd);

int main(int argc, char* argv[])
{
	std::cout << "Notes:" << std::endl;
	std::cout << "-> The assumption is Y is height, so in MeshLab, " << std::endl;
	std::cout << "   rotate by -90 degrees along the x axis." << std::endl;
	std::cout << "-> The ply model should NOT have normals, so" << std::endl;
	std::cout << "   you will have to regenerated them in MeshLab." << std::endl;
	std::cout << std::endl;

	if ( (argc < 2) || (argc > 4 ) )
	{
		std::cout << "Usage: program.exe source.ply dest.ply waterHeightRatio" << std::endl;
		std::cout << "(waterHeightRatio is from 0.0 to 1.0 in relation to the height)" << std::endl;
		std::cout << std::endl;
		return -1;
	}

	float waterHeightRatio = 0.5f;		// Default
	std::string outputFileName = "output.ply";

	if ( argc == 2 )
	{
		std::cout << "Usage: program.exe source.ply dest.ply waterHeightRatio" << std::endl;
		std::cout << "(waterHeightRatio is from 0.0 to 1.0 in relation to the height)" << std::endl;
		std::cout << std::endl;
		std::cout << "Assuming output file is: output.ply" << std::endl;
		std::cout << "Assuming waterHeightRatio = 0.5" << std::endl;
	}
	else if ( argc == 3 ) 
	{
		std::cout << "Usage: program.exe source.ply dest.ply waterHeightRatio" << std::endl;
		std::cout << "(waterHeightRatio is from 0.0 to 1.0 in relation to the height)" << std::endl;
		std::cout << std::endl;
		std::cout << "Assuming waterHeightRatio = 0.5" << std::endl;
		outputFileName.clear();
		outputFileName.append( argv[2] );
	}
	else if ( argc == 4 )  
	{
		outputFileName.clear();
		outputFileName.append( argv[2] );
		waterHeightRatio = (float)atof( argv[3] );
	}


	std::cout << "Input file: " << argv[1] << std::endl;
	std::cout << "Output file: " << outputFileName << std::endl;
	std::cout << "waterHeightRatio: " << waterHeightRatio << std::endl;
	std::cout << std::endl;

	std::ifstream inFile(argv[1]);
	if ( ! inFile.is_open() )
	{
		std::cout << "Can't open file: " << argv[1] << std::endl;
		return -1;
	}

	ReadToToken( "vertex", inFile );
	unsigned int numVerts;
	inFile >> numVerts;

	ReadToToken( "face", inFile );
	unsigned int numFaces;
	inFile >> numFaces;

	ReadToToken( "end_header", inFile );



	std::vector<sVertex> vecVerts;
	std::vector<glm::ivec3> vecTris;

	std::cout << "Loading file...";

	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		sVertex tempV;
		inFile 
			>> tempV.pos.x >> tempV.pos.y >> tempV.pos.z;
		vecVerts.push_back( tempV );
	}

	for ( unsigned int index = 0; index != numFaces; index++ )
	{
		glm::ivec3 tempTri;
		int garbage;
		inFile >> garbage >> tempTri.x >> tempTri.y >> tempTri.z;
		vecTris.push_back(tempTri);
	}

	inFile.close();


	std::cout << "OK" << std::endl;

	// Scale by 400x 
	for ( sVertex& curVert : vecVerts )
	{
		curVert.pos.x *= 400.0f;
		curVert.pos.y *= 400.0f;
		curVert.pos.z *= 400.0f;
	}


	// Find extents
	sLimits origLimits;
	CalcExtents( vecVerts, origLimits );


	// Stage 1: Pick the water height (which is the y axis)
	float waterHeightRelative = waterHeightRatio * origLimits.extentXYZ.y;
	float waterHeightAbsolute = waterHeightRelative + origLimits.minXYZ.y;
	
	std::cout << "Lowest height = " << origLimits.minXYZ.y << std::endl;
	std::cout << "Highest height = " << origLimits.maxXYZ.y << std::endl;
	std::cout << "Water heigth:" << std::endl;
	std::cout << "\tRelative: " << waterHeightRelative << std::endl;
	std::cout << "\tAbsolute: " << waterHeightAbsolute << std::endl;

	// Shift the model to the centre...
	glm::vec3 shiftToCentreXYZ;
	shiftToCentreXYZ.x = -((origLimits.extentXYZ.x / 2.0f) + origLimits.minXYZ.x);
	shiftToCentreXYZ.y = -waterHeightAbsolute;
	shiftToCentreXYZ.z = -((origLimits.extentXYZ.z / 2.0f) + origLimits.minXYZ.z);

	std::cout << std::endl;
	std::cout << "Shifting the model to the origin:" << std::endl;
	std::cout << "\t" << shiftToCentreXYZ.x << ", " << shiftToCentreXYZ.y << ", " << shiftToCentreXYZ.z << std::endl;
	std::cout << std::endl;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		vecVerts[index].pos += shiftToCentreXYZ;
	}

	// Reajust extents...
	// Assume 1st vertex is both max and min (for now)
	sLimits adjLimits;
	CalcExtents( vecVerts, adjLimits );

	// Flattening any terrain below water level...
	std::cout << "Flattening anything below sealevel... ";
	unsigned int numVertsFlattened = 0;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		if ( vecVerts[index].pos.y < 0.0f )
		{
			vecVerts[index].pos.y = 0.0f;

			numVertsFlattened++;
		}
	}
	std::cout << numVertsFlattened << " vertices set to y=0" << std::endl;


	// Step 2: Determine if height is outside of sphere
	sSphere theSphere;
	theSphere.c.x = (adjLimits.extentXYZ.x / 2.0f) + adjLimits.minXYZ.x;
	theSphere.c.y = -(adjLimits.maxExtent / 2.0f);
	theSphere.c.z = (adjLimits.extentXYZ.z / 2.0f) + adjLimits.minXYZ.z;

	theSphere.r = adjLimits.maxExtent * 0.70f;

	std::cout << "Rounding terrain... ";
	unsigned int numVertsScaled = 0;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		if ( vecVerts[index].pos.y > 0.0f )
		{
			// // Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
			// returns t value of intersection and intersection point q

			glm::vec3 rayStart = vecVerts[index].pos;
			rayStart.y = origLimits.maxExtent;
			glm::vec3 rayDirection = glm::vec3(0.0f, -1.0f, 0.0f);
			float t = origLimits.maxExtent; 
			glm::vec3 intersectionPoint;

			if ( IntersectRaySphere( rayStart, rayDirection, theSphere, t, intersectionPoint ) )
			{
				// Scale height based on curve of sphere
				//float quaterMaxExtent = maxExtent/4.0f;

				//float ratio = (intersectionPoint.y / quaterMaxExtent);
				float ratio = intersectionPoint.y / origLimits.extentXYZ.y;

				vecVerts[index].pos.y *= ratio;
//				vecVerts[index].y = intersectionPoint.y;
			}
			else
			{
				vecVerts[index].pos.y = 0.0f;
			}
		}

		// Check if height is LT 0.0f
		if ( vecVerts[index].pos.y < 0.0f )
		{
			vecVerts[index].pos.y = 0.0f;
		}
	}
	std::cout << "done." << std::endl;


	sLimits resLimits;
	CalcExtents( vecVerts, resLimits );
	float ratioAdjust = origLimits.extentXYZ.y / resLimits.extentXYZ.y;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		vecVerts[index].pos.y *= ratioAdjust;
	}

	sLimits currentLimits;
	CalcExtents(vecVerts, currentLimits);

	// Expanding the water
	std::cout << "Expanding the water...";
	// Go through the triangles and move the 'edges' out some distance from the centre.
	// Choose larger of x or y
	float newWaterEdgeDistance = glm::max(currentLimits.extentXYZ.x, currentLimits.extentXYZ.z) * 3.0f;

	float shortestTriangleEdgeLength = currentLimits.extentXYZ.x;
	// Calculate smallest edge length of the triangles (i.e. the smallest length of any triangle)
	for ( glm::ivec3 curTri : vecTris )
	{
		glm::vec3 verts[3];
		verts[0] = vecVerts[curTri.x].pos;
		verts[1] = vecVerts[curTri.y].pos;
		verts[2] = vecVerts[curTri.z].pos;

		shortestTriangleEdgeLength = glm::min( glm::distance( verts[0], verts[1] ), shortestTriangleEdgeLength );
		shortestTriangleEdgeLength = glm::min( glm::distance( verts[1], verts[2] ), shortestTriangleEdgeLength );
		shortestTriangleEdgeLength = glm::min( glm::distance( verts[2], verts[0] ), shortestTriangleEdgeLength );
	}
	std::cout << "Shortest triangle edge length = " << shortestTriangleEdgeLength << std::endl;

	float edgeEpsilon = shortestTriangleEdgeLength / 3.0f;

	// Find centre of water - i.e. ignore y centre value (which is half the mountain height)
	glm::vec3 centreXZ = glm::vec3(currentLimits.centreXYZ.x, 0.0f, currentLimits.centreXYZ.z);

	for ( sVertex &curVertex : vecVerts )
	{
		// This vertex on the 'edge' (almost or at extent in x or z)?
		// 
		// Note: if there's any vertex at the "edge" then either x or z is "close"
		// If they're at the edge, then extend them out to the edge of a circle 
		//	of radius newWaterEdgeDistance.
		// But if we've already moved an x, then we don't need to move a z
		//
		if ( curVertex.pos.x < (currentLimits.minXYZ.x + edgeEpsilon) )
		{
			curVertex.pos = glm::normalize(curVertex.pos - centreXZ) * newWaterEdgeDistance;
			continue;
		}
		if ( curVertex.pos.x > (currentLimits.maxXYZ.x - edgeEpsilon) )
		{
			curVertex.pos = glm::normalize(curVertex.pos - centreXZ) * newWaterEdgeDistance;
			continue;
		}
		if (curVertex.pos.z < (adjLimits.minXYZ.z + edgeEpsilon))
		{
			curVertex.pos = glm::normalize(curVertex.pos - centreXZ) * newWaterEdgeDistance;
			continue;
		}
		if (curVertex.pos.z > (adjLimits.maxXYZ.z - edgeEpsilon))
		{
			curVertex.pos = glm::normalize(curVertex.pos - centreXZ) * newWaterEdgeDistance;
			continue;
		}
	}

	std::cout << "done." << std::endl;

	// Apply UVs on X-Z plane
	// Water edge is newWaterEdgeDistance from centre
	std::cout << "Generating texture coords";
	float ModelExtent = newWaterEdgeDistance * 2.0f;
	for ( sVertex& curVertex : vecVerts )
	{
		curVertex.UV.x = (curVertex.pos.x + newWaterEdgeDistance) / ModelExtent;
		curVertex.UV.y = (curVertex.pos.z + newWaterEdgeDistance) / ModelExtent;
	}
	std::cout << "done." << std::endl;


	
//	simplifyWaterMesh(vecVerts, vecTris);


	// Colouring the grass and water...
	std::cout << "Colouring the grass and water...";
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		if ( vecVerts[index].pos.y < 0.1f )
		{
			// Water blue: 62,164,240
			vecVerts[index].rgba = glm::vec4( HTML_to_RGB( glm::vec3( 62.0f, 164.0f, 240.0f)), 1.0f);
		}
		else
		{
			// Grass green: 19,109,21
			vecVerts[index].rgba = glm::vec4( HTML_to_RGB( glm::vec3( 19.0f, 109.0f, 21.0f)), 1.0f);
		}
	}
	std::cout << "done." << std::endl;






	std::cout << "Generating smooth vertex normals...";

	// Clear normals
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		vecVerts[index].norm = glm::vec3(0.0f);
	}

	// Accumulate face normals onto vertex normals
	for ( unsigned int faceIndex = 0; faceIndex != numFaces; faceIndex++ )
	{
		glm::ivec3 curTri = vecTris[faceIndex];

		glm::vec3 v0 = vecVerts[curTri.x].pos;
		glm::vec3 v1 = vecVerts[curTri.y].pos;
		glm::vec3 v2 = vecVerts[curTri.z].pos;

		glm::vec3 faceNormal = glm::cross( v1 - v0, v2 - v0);
		faceNormal = glm::normalize(faceNormal);

		vecVerts[curTri.x].norm += faceNormal;
		vecVerts[curTri.y].norm += faceNormal;
		vecVerts[curTri.z].norm += faceNormal;
	}

	// Average (normalize) accumulated vertex normals
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		vecVerts[index].norm = glm::normalize(vecVerts[index].norm);
	}

	std::cout << "done." << std::endl;
	std::cout << std::endl;




	std::cout << "Outputting to file " << outputFileName << "...";
	std::ofstream outFile( outputFileName.c_str() );
	if ( ! outFile.is_open() )
	{
		std::cout << "Can't open file for writing." << std::endl;
		return -1;
	}

	outFile << "ply" << std::endl;
	outFile << "format ascii 1.0" << std::endl;
	outFile << "comment Terrain_to_Island_Converter_2 generated" << std::endl;
	outFile << "comment TextureFile Uvrefmap_checkeredmap.jpg" << std::endl;
	outFile << "element vertex "<< numVerts << std::endl;
	outFile << "property float x" << std::endl;
	outFile << "property float y" << std::endl;
	outFile << "property float z" << std::endl;
	outFile << "property float nx" << std::endl;
	outFile << "property float ny" << std::endl;
	outFile << "property float nz" << std::endl;
	outFile << "property uchar red" << std::endl;
	outFile << "property uchar green" << std::endl;
	outFile << "property uchar blue" << std::endl;
	outFile << "property uchar alpha" << std::endl;
	outFile << "property float texture_u" << std::endl;
	outFile << "property float texture_v" << std::endl;
	outFile << "element face " << numFaces << std::endl;
	outFile << "property list uchar int vertex_indices" << std::endl;
	outFile << "end_header" << std::endl;

	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		outFile 
			<< vecVerts[index].pos.x << " " 
			<< vecVerts[index].pos.y << " " 
			<< vecVerts[index].pos.z << " " 

			<< vecVerts[index].norm.x << " " 
			<< vecVerts[index].norm.y << " " 
			<< vecVerts[index].norm.z << " "

			<< (unsigned int)(vecVerts[index].rgba.r * 255.0f) << " "
			<< (unsigned int)(vecVerts[index].rgba.g * 255.0f) << " " 
			<< (unsigned int)(vecVerts[index].rgba.b * 255.0f) << " " 
			<< "255" << " "
			
			<< vecVerts[index].UV.x << " "
			<< vecVerts[index].UV.y << std::endl;
	}

	for ( unsigned int index = 0; index != numFaces; index++ )
	{
		outFile 
			<< "3 " 
			<< vecTris[index].x << " "
			<< vecTris[index].y << " "
			<< vecTris[index].z << std::endl;
	}

	outFile.close();

	std::cout << std::endl;
	std::cout << std::endl;
	system("pause");

	return 0;
}

// Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
// returns t value of intersection and intersection point q
typedef glm::vec3 Point;
typedef glm::vec3 Vector;

bool IntersectRaySphere(Point p, Vector d, sSphere s, float &t, Point &q)
{
	Vector m = p - s.c;

    float b = glm::dot(m, d);
	float c = glm::dot(m, m) - s.r * s.r;
    // Exit if r’s origin outside s (c > 0)and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f) 
	{
		return false;
	}
    float discr = b*b - c;
    // A negative discriminant corresponds to ray missing sphere
    if (discr < 0.0f) 
	{
		return false;
	}
    // Ray now found to intersect sphere, compute smallest t value of intersection
    t = -b - sqrt(discr);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t < 0.0f)
	{
		t = 0.0f;
	}
	else
	{
		t = t;
	}
    q = p + t * d;
    return true;
}

void CalcExtents( std::vector<sVertex> &vecVerts, sLimits &limits )
{
	// max and min are 1st vertes
	limits.minXYZ = limits.maxXYZ = vecVerts[0].pos;

	for ( unsigned int index = 0; index != vecVerts.size(); index++ )
	{
		limits.minXYZ.x = getMin( vecVerts[index].pos.x, limits.minXYZ.x );
		limits.minXYZ.y = getMin( vecVerts[index].pos.y, limits.minXYZ.y );
		limits.minXYZ.z = getMin( vecVerts[index].pos.z, limits.minXYZ.z );

		limits.maxXYZ.x = getMax( vecVerts[index].pos.x, limits.maxXYZ.x );
		limits.maxXYZ.y = getMax( vecVerts[index].pos.y, limits.maxXYZ.y );
		limits.maxXYZ.z = getMax( vecVerts[index].pos.z, limits.maxXYZ.z );
	}

	limits.extentXYZ.x = limits.maxXYZ.x - limits.minXYZ.x;
	limits.extentXYZ.y = limits.maxXYZ.y - limits.minXYZ.y;		// height 
	limits.extentXYZ.z = limits.maxXYZ.z - limits.minXYZ.z;

	limits.maxExtent = limits.extentXYZ.x;
	limits.maxExtent = getMax( limits.extentXYZ.y, limits.maxExtent );
	limits.maxExtent = getMax( limits.extentXYZ.z, limits.maxExtent );

	limits.centreXYZ.x = limits.minXYZ.x + (limits.extentXYZ.x / 2.0f);
	limits.centreXYZ.y = limits.minXYZ.y + (limits.extentXYZ.y / 2.0f);
	limits.centreXYZ.z = limits.minXYZ.z + (limits.extentXYZ.z / 2.0f);

	return;
}

glm::vec3 HTML_to_RGB(glm::vec3 HTML_Colour)
{
	return glm::vec3( HTML_Colour.r / 255.0f, HTML_Colour.g / 255.0f, HTML_Colour.b / 255.0f);
}

// Will remove any triangles associated with this vertex from the vecTriSource
//	and place them on the vecTrisRemoved.
// Returns how many triangles were removed.
unsigned int removeAssociatedTriangles(unsigned int vertIndex,
							   std::vector<glm::ivec3>& vecTriSource,
							   std::vector<glm::ivec3>& vecRemovedTriangles)
{
	vecRemovedTriangles.clear();
	std::vector<glm::ivec3> vecTriSource_WITH_TrisRemoved;

	unsigned int numTriangleRemoved = 0;

	for ( glm::ivec3 curTri : vecTriSource )
	{
		if ( ( curTri.x == vertIndex) || (curTri.y == vertIndex) || (curTri.z == vertIndex) )
		{
			// Is associated with this vertex
			vecRemovedTriangles.push_back(curTri);
			numTriangleRemoved++;
		}
		else
		{
			// ISN'T associated
			vecTriSource_WITH_TrisRemoved.push_back(curTri);
		}
	}//for ( glm::ivec3 curTri

	// Copy them back
	vecTriSource.clear();
	for ( glm::ivec3 curTri : vecTriSource_WITH_TrisRemoved )
	{
		vecTriSource.push_back(curTri);
	}

	return numTriangleRemoved;
}

void appendTriangles(std::vector<glm::ivec3>& vecTris, std::vector<glm::ivec3>& vecTrisToAdd)
{
	for ( glm::ivec3 curTri : vecTrisToAdd )
	{
		vecTris.push_back(curTri);
	}
	return;
}

// TODO: 
void simplifyWaterMesh( std::vector<sVertex> &vecVerts, std::vector<glm::ivec3>& vecTris)
{
	std::cout << "Simplifying water mesh...";

	// Go through all the vertices. 
	// If it's at the "water circle" edge, skip it. (newWaterEdgeDistance from centre)
	// If the triangles associated with this are all "water" (i.e. flat), then remove this vertex.
	bool bKeepSimplifying = true;
	unsigned int loopCount = 0;
	while (bKeepSimplifying)
	{
		for (unsigned int vertIndex = 0; vertIndex != vecVerts.size(); vertIndex++)
		{
			std::vector<glm::ivec3> vecAssociatedTriangles;

			if (removeAssociatedTriangles(vertIndex, vecTris, vecAssociatedTriangles) > 0)
			{
	//				std::cout << "Associated Tris " << vecAssociatedTriangles.size() << std::endl;

				appendTriangles(vecTris, vecAssociatedTriangles);
			}

	//			if ( areAdjacentTrianglesFlat( vecVerts, vecTris, ) )
	//			{
	//				removeVertex(vecVerts, vecTris);
	//			}
		}

		bKeepSimplifying = false;


	}//while (bKeepSimplifying)

	std::cout << "done." << std::endl;


return;
}

// Takes a modified PLY (no headers) and generates a binary resource file
// Ply is this format:
//	ply
//	format ascii 1.0
//	comment VCGLIB generated
//	element vertex 461
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
//	property float texture_v
//	element face 208
//	property list uchar int vertex_indices
//	end_header
// 
// But stripped of header, with only number of vertices and faces kept on 1st line
bool DataGen(std::string fileIn, std::string fileOut)
{
	std::ifstream datIn("Source_Dat.txt");
	if ( ! datIn.is_open() )
	{
		return false;
	}

	return true;
}