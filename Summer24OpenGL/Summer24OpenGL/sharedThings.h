#pragma once

#include "cLightManager.h"
#include <vector>
#include "cMeshObject.h"
#include "cBasicFlyCamera/cBasicFlyCamera.h"

//extern glm::vec3 g_cameraEye;//  = glm::vec3(0.0, 0.0, -20.0f);
extern cBasicFlyCamera* g_pFlyCamera;

// note that this is a pointer because 
extern std::vector< cMeshObject* > g_MeshesToDraw;

extern cMeshObject* g_pSmoothSphere;

extern cLightManager* g_pLights;// = NULL;

