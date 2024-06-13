#include "globalOpenGL.h"
#include "sharedThings.h"

#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cMeshObject.h"

#include <iostream>

extern cVAOManager* g_pMeshManager;

void LoadTextures(cBasicTextureManager* pTextureManager)
{
    pTextureManager->SetBasePath("assets/textures");
    if (pTextureManager->Create2DTextureFromBMPFile("beyonce.bmp", true))
    {
        std::cout << "Loaded beyonce.bmp OK" << std::endl;
    }
    //if ( ::g_TextureManager->Create2DTextureFromBMPFile("Dungeons_2_Texture_01_A.png", true) )
    //{
    //    std::cout << "Dungeons_2_Texture_01_A.png OK" << std::endl;
    //}
    if (pTextureManager->Create2DTextureFromBMPFile("Dungeons_2_Texture_01_A.bmp", true))
    {
        std::cout << "Dungeons_2_Texture_01_A.bmp OK" << std::endl;
    }

    // You can load as many textures as you have RAM 
    pTextureManager->Create2DTextureFromBMPFile("SlothInSpace.bmp", true);

    // You can load as many textures as you have RAM 
    pTextureManager->Create2DTextureFromBMPFile("white-brick-wall-seamless-texture-free.bmp", true);


    return;
}


void LoadFilesIntoVAOManager(GLuint program)
{
//    sModelDrawInfo meshInfoCow;
//    if (!::g_pMeshManager->LoadModelIntoVAO("assets/models/cow_xyz_n_rgba.ply", meshInfoCow, program))
//    {
//        std::cout << "ERROR: Didn't load the cow" << std::endl;
//    }
//
//    sModelDrawInfo carMeshInfo;
//    if (!::g_pMeshManager->LoadModelIntoVAO("assets/models/de--lorean_xyz_n_rgba.ply", carMeshInfo, program))
//    {
//        std::cout << "ERROR: Didn't load the cow" << std::endl;
//    }
//
//    sModelDrawInfo teapotMeshInfo;
//    if (!::g_pMeshManager->LoadModelIntoVAO("assets/models/Utah_Teapot_xyz_rgba.ply", teapotMeshInfo, program))
//    {
//        std::cout << "ERROR: Didn't load the cow" << std::endl;
//    }
//
//    sModelDrawInfo dolphinMesh;
//    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/dolphin_xyz_n_rgba.ply", dolphinMesh, program))
//    {
//        std::cout << "loaded: "
//            << dolphinMesh.meshName << " "
//            << dolphinMesh.numberOfVertices << " vertices" << std::endl;
//    }
//
//    sModelDrawInfo terrainMesh;
////    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/fractalTerrainMeshLab_xyz_n_rgba.ply", terrainMesh, program))
//    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/output.ply", terrainMesh, program))
//    {
//        std::cout << "loaded: "
//            << terrainMesh.meshName << " "
//            << terrainMesh.numberOfVertices << " vertices" << std::endl;
//    }
//
//
//    sModelDrawInfo wearhouseMesh;
//    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Warehouse_xyz_n_rgba.ply", wearhouseMesh, program))
//    {
//        std::cout << "loaded: "
//            << wearhouseMesh.meshName << " "
//            << wearhouseMesh.numberOfVertices << " vertices" << std::endl;
//    }

    sModelDrawInfo sphereMesh;
    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Isoshphere_flat_4div_xyz_n_rgba_uv.ply", sphereMesh, program))
    {
        std::cout << "loaded: "
            << sphereMesh.meshName << " "
            << sphereMesh.numberOfVertices << " vertices" << std::endl;
    }

    sModelDrawInfo sphereMeshInverted;
    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Isoshphere_smooth_inverted_normals_xyz_n_rgba_uv.ply", sphereMesh, program))
    {
        std::cout << "loaded: "
            << sphereMeshInverted.meshName << " "
            << sphereMeshInverted.numberOfVertices << " vertices" << std::endl;
    }
//
//    sModelDrawInfo skullMesh;
//    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Dungeon_models/Dead bodies, etc/SM_Item_Skull_01_no_UV.ply", sphereMesh, program))
//    {
//        std::cout << "loaded: "
//            << skullMesh.meshName << " "
//            << skullMesh.numberOfVertices << " vertices" << std::endl;
//    }
//    sModelDrawInfo floorlMesh;
//    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Dungeon_models/Floors/SM_Env_Dwarf_Floor_07_no_UV.ply", sphereMesh, program))
//    {
//        std::cout << "loaded: "
//            << floorlMesh.meshName << " "
//            << floorlMesh.numberOfVertices << " vertices" << std::endl;
//    }


    sModelDrawInfo DwarfMesh;
    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Dungeon_models/Dead bodies, etc/SM_Prop_DeadBody_Dwarf_01.ply", DwarfMesh, program))
    {
        std::cout << "loaded: "
            << DwarfMesh.meshName << " "
            << DwarfMesh.numberOfVertices << " vertices" << std::endl;
    }

    sModelDrawInfo terrainMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Large_Terrain.ply", terrainMesh, program);

    sModelDrawInfo teapotMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Utah_Teapot_xyz_n_rgba_uv.ply",
                                       teapotMesh,
                                       program,
                                       true);       // Generate spherical UVs

    sModelDrawInfo flatQuad;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/LargeFlatQuad.ply",
                                       flatQuad,
                                       program);     

    return;
}

void LoadModelsIntoScene(void)
{

    cMeshObject* pTeaPot = new cMeshObject();
    pTeaPot->meshFileName = "assets/models/Utah_Teapot_xyz_n_rgba_uv.ply";
    pTeaPot->textureNames[0] = "beyonce.bmp";
    pTeaPot->scale = 0.75f;
    pTeaPot->orientation.x = glm::radians(-90.0f);
    pTeaPot->position.z = 50.0f;
    ::g_MeshesToDraw.push_back(pTeaPot);


    cMeshObject* pDwarf = new cMeshObject();
    pDwarf->meshFileName = "assets/models/Dungeon_models/Dead bodies, etc/SM_Prop_DeadBody_Dwarf_01.ply";
    pDwarf->scale = 0.1f;
//    pDwarf->bDoNotLight = true;
    pDwarf->position.z = 10.0f;
    pDwarf->position.y = -10.0f;
    pDwarf->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
    pDwarf->textureBlendRatio[0] = 1.0f;

    ::g_MeshesToDraw.push_back(pDwarf);

    cMeshObject* pDwarf2 = new cMeshObject();
    pDwarf2->meshFileName = "assets/models/Dungeon_models/Dead bodies, etc/SM_Prop_DeadBody_Dwarf_01.ply";
    pDwarf2->scale = 0.1f;
//    pDwarf2->bDoNotLight = true;
    pDwarf2->position.z = 10.0f;
    pDwarf2->position.x = 15.0f;
    pDwarf2->position.y = -10.0f;

    pDwarf2->textureNames[0] = "beyonce.bmp";
    pDwarf2->textureBlendRatio[0] = 1.0f;

    pDwarf2->bUseTextureAsColour = true;
    ::g_MeshesToDraw.push_back(pDwarf2);


    cMeshObject* pGround = new cMeshObject();
 //   pGround->meshFileName = "assets/models/Large_Terrain.ply";
    pGround->meshFileName = "assets/models/LargeFlatQuad.ply";
//    pGround->texture00Name = "SlothInSpace.bmp";

    pGround->textureNames[0] = "white-brick-wall-seamless-texture-free.bmp";
    pGround->textureBlendRatio[0] = 1.0f;

    pGround->textureNames[1] = "beyonce.bmp";
    pGround->textureBlendRatio[1] = 0.0f;

    pGround->bUseTextureAsColour = true;
    pGround->position.y = -40.0f;
    pGround->friendlyName = "The Ground";
    ::g_MeshesToDraw.push_back(pGround);

    // 
    // 
    //cMeshObject* pFloor = new cMeshObject();
    //pFloor->meshFileName = "assets/models/Dungeon_models/Floors/SM_Env_Dwarf_Floor_07_no_UV.ply";
    ////pFloor->bIsWireFrame = true;
    //pFloor->position.y = -5.0f;
    //pFloor->position.x = 10.0f;
    //pFloor->bOverrideVertexModelColour = true;
    //pFloor->colourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    //pFloor->scale = 0.025f;
    //::g_MeshesToDraw.push_back(pFloor);

    //cMeshObject* pSkull = new cMeshObject();
    //pSkull->meshFileName = "assets/models/Dungeon_models/Dead bodies, etc/SM_Item_Skull_01_no_UV.ply";
    ////pSkull->bIsWireFrame = true;
    //pSkull->bOverrideVertexModelColour = true;
    //pSkull->colourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    //pSkull->scale = 15.0f;
    //::g_MeshesToDraw.push_back(pSkull);


    ::g_pSmoothSphere = new cMeshObject();
    ::g_pSmoothSphere->meshFileName = "assets/models/Isoshphere_flat_4div_xyz_n_rgba_uv.ply";
//    ::g_pSmoothSphere->meshFileName = "assets/models/Isoshphere_smooth_inverted_normals_xyz_n_rgba.ply";
    ::g_pSmoothSphere->bIsWireFrame = true;
    ::g_pSmoothSphere->bOverrideVertexModelColour = true;
    ::g_pSmoothSphere->colourRGB = glm::vec3(1.0f);
    ::g_pSmoothSphere->scale = 1.0f;
    ::g_pSmoothSphere->bIsVisible = true;
//    ::g_MeshesToDraw.push_back(::g_pSmoothSphere);



    cMeshObject* pCow2 = new cMeshObject();
    pCow2->meshFileName = "assets/models/cow_xyz_n_rgba.ply";
//    pCow2->bIsWireFrame = false;
    pCow2->position.x = +10.0f;
    pCow2->scale = 0.5f;
    pCow2->orientation.z = glm::radians(-45.0f);
    pCow2->bOverrideVertexModelColour = true;
    pCow2->colourRGB = glm::vec3(0.0f, 1.0f, 0.0f);
    ::g_MeshesToDraw.push_back(pCow2);


    cMeshObject* pCar = new cMeshObject();
    pCar->meshFileName = "assets/models/de--lorean_xyz_n_rgba.ply";
    pCar->orientation.x = glm::radians(-90.0f);
    //pCar->position.z = 0.0f;
    //pCar->position.y = 0.0f;
    pCar->scale = 0.5f;
    pCar->shinniness = 1000.0f;  // 1 = 'flat' like dry clay -- to millions
//    pCar->bIsWireFrame = true;
    pCar->bIsVisible = true;
    pCar->alphaTransparency = 0.5f;
    ::g_MeshesToDraw.push_back(pCar);

    //cMeshObject* pDolphin = new cMeshObject();
    //pDolphin->meshFileName = "assets/models/dolphin_xyz_n_rgba.ply";
    //pDolphin->scale = 0.01f;
    //::g_MeshesToDraw.push_back(pDolphin);


    cMeshObject* pTerrain = new cMeshObject();
    pTerrain->meshFileName = "assets/models/fractalTerrainMeshLab_xyz_n_rgba.ply";
    //pTerrain->meshFileName = "assets/models/output.ply";
    pTerrain->position.y = -30.0f;
//    pTerrain->bIsWireFrame = true;
    ::g_MeshesToDraw.push_back(pTerrain);

    cMeshObject* pWarehouse = new cMeshObject();
    pWarehouse->meshFileName = "assets/models/Warehouse_xyz_n_rgba.ply";
//    pWarehouse->bIsWireFrame = true;
    pWarehouse->position.y = -10.0f;
    pWarehouse->orientation.y = glm::radians(-90.0f);
    //
    pWarehouse->bOverrideVertexModelColour = true;
    // rgb(68, 109, 122)
    pWarehouse->colourRGB = glm::vec3(68.0f / 255.0f, 109.0f / 255.0f, 122.0f / 255.0f);
    ::g_MeshesToDraw.push_back(pWarehouse);

   // Load the models I'd like to draw in the scene
    cMeshObject* pCow = new cMeshObject();
    pCow->meshFileName = "assets/models/cow_xyz_n_rgba.ply";
    pCow->bIsWireFrame = false;
    pCow->position.x = -10.f;
    pCow->friendlyName = "George";
    pCow->alphaTransparency = 0.65f;
    ::g_MeshesToDraw.push_back(pCow);



    return;
}
