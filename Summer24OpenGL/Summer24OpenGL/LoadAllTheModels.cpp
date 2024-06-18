#include "globalOpenGL.h"
#include "sharedThings.h"

#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cMeshObject.h"

#include <iostream>

extern cVAOManager* g_pMeshManager;

// This will be our "skybox" object
cMeshObject* g_pSkyBoxSphere = NULL;

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


    // Load the cube map (skybox) texture
    pTextureManager->SetBasePath("assets/textures/cubemap_textures");
    std::string loadError;
    if ( pTextureManager->CreateCubeTextureFromBMPFiles( "SunnyDay", 
                                                        "TropicalSunnyDayRight2048.bmp",
                                                        "TropicalSunnyDayLeft2048.bmp",
                                                        "TropicalSunnyDayUp2048.bmp",
                                                        "TropicalSunnyDayDown2048.bmp",
                                                        "TropicalSunnyDayFront2048.bmp",
                                                        "TropicalSunnyDayBack2048.bmp", 
                                                        true,
                                                        loadError) )
    {
        std::cout << "Tropical sunny day cube map loaded OK" << std::endl;
    }
    else
    {
        std::cout << "Error: Didn't load cube map because: " << loadError << std::endl;
    }

    if ( pTextureManager->CreateCubeTextureFromBMPFiles( "Space", 
                                                        "SpaceBox_right1_posX.bmp",
                                                        "SpaceBox_left2_negX.bmp",
                                                        "SpaceBox_top3_posY.bmp",
                                                        "SpaceBox_bottom4_negY.bmp",
                                                        "SpaceBox_front5_posZ.bmp",
                                                        "SpaceBox_back6_negZ.bmp", 
                                                        true,
                                                        loadError) )
    {
        std::cout << "Space cube map loaded OK" << std::endl;
    }
    else
    {
        std::cout << "Error: Didn't load cube map because: " << loadError << std::endl;
    }


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

//    sModelDrawInfo cameraMesh;
//    ::g_pMeshManager->LoadModelIntoVAO("assets/models/video_camera_aligned_to_z_zxis_WITH_sphere.ply",
//                                       cameraMesh,
//                                       program);     
    sModelDrawInfo ballInFrontOfCameraMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Isoshphere_in_front_of_camera.ply",
                                       ballInFrontOfCameraMesh,
                                       program);     
    sModelDrawInfo cameraMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/video_camera_aligned_to_z_zxis_triangles.ply",
                                       cameraMesh,
                                       program);     

    sModelDrawInfo DeadBody_NomadMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Dungeon_models/Dead bodies, etc/SM_Prop_DeadBody_Nomad_02.ply",
                                       DeadBody_NomadMesh,
                                       program);     
    sModelDrawInfo SM_Env_Crystals_Cluster_Large_01Mesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Dungeon_models/Crystals/SM_Env_Crystals_Cluster_Large_01.ply",
                                       SM_Env_Crystals_Cluster_Large_01Mesh,
                                       program);     

    // Load the skybox object
    sModelDrawInfo skyboxSphereMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Isoshphere_smooth_inverted_normals_xyz_n_rgba_uv.ply",
                                       skyboxSphereMesh,
                                       program);     


    return;
}

void LoadModelsIntoScene(void)
{

    ::g_pSkyBoxSphere = new cMeshObject();
    ::g_pSkyBoxSphere->meshFileName = "assets/models/Isoshphere_smooth_inverted_normals_xyz_n_rgba_uv.ply";
    //::g_pSkyBoxSphere->bIsWireFrame = true;
    //::g_pSkyBoxSphere->bDoNotLight = true;
    //::g_pSkyBoxSphere->bOverrideVertexModelColour = true;
    //::g_pSkyBoxSphere->colourRGB = glm::vec3(0.0f, 1.0f, 1.0f);
    //::g_pSkyBoxSphere->bUseTextureAsColour = false;
    //::g_pSkyBoxSphere->position.x = 5.0f;
    //::g_pSkyBoxSphere->scale = 10.0f;
    //::g_MeshesToDraw.push_back(::g_pSkyBoxSphere);




    cMeshObject* pCameraModel = new cMeshObject();
//    pCameraModel->meshFileName = "assets/models/video_camera_aligned_to_z_zxis_WITH_sphere.ply";
    pCameraModel->meshFileName = "assets/models/video_camera_aligned_to_z_zxis_triangles.ply";
    pCameraModel->bIsWireFrame = true;
    pCameraModel->bDoNotLight = true;
    pCameraModel->bOverrideVertexModelColour = true;
    pCameraModel->colourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pCameraModel->bUseTextureAsColour = false;
    pCameraModel->friendlyName = "Camera";
    //pCameraModel->textureNames[0] = "beyonce.bmp";
    //pCameraModel->scale = 0.75f;
    ::g_MeshesToDraw.push_back(pCameraModel);

    cMeshObject* pCameraTargetBall = new cMeshObject();
    pCameraTargetBall->meshFileName = "assets/models/Isoshphere_in_front_of_camera.ply";
    pCameraTargetBall->bIsWireFrame = true;
    pCameraTargetBall->bDoNotLight = true;
    pCameraTargetBall->bOverrideVertexModelColour = true;
    pCameraTargetBall->colourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pCameraTargetBall->bUseTextureAsColour = false;
    pCameraTargetBall->friendlyName = "CameraTargetBall";
    ::g_MeshesToDraw.push_back(pCameraTargetBall);


    cMeshObject* pTeaPot = new cMeshObject();
    pTeaPot->meshFileName = "assets/models/Utah_Teapot_xyz_n_rgba_uv.ply";
    pTeaPot->textureNames[0] = "beyonce.bmp";
    pTeaPot->textureBlendRatio[0] = 1.0f;
    pTeaPot->scale = 0.75f;
    pTeaPot->orientation.x = glm::radians(-90.0f);
    pTeaPot->position.z = 150.0f;
    pTeaPot->shinniness = 100.0f;
    ::g_MeshesToDraw.push_back(pTeaPot);


    cMeshObject* pDwarf = new cMeshObject();
    pDwarf->meshFileName = "assets/models/Dungeon_models/Dead bodies, etc/SM_Prop_DeadBody_Dwarf_01.ply";
    pDwarf->scale = 0.1f;
//    pDwarf->bDoNotLight = true;
    pDwarf->position.z = 60.0f;
    pDwarf->position.y = -10.0f;
    pDwarf->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
    pDwarf->textureBlendRatio[0] = 1.0f;

    pDwarf->specularHighlightColour = glm::vec3(0.0f, 0.0f, 0.0f);
    pDwarf->shinniness = 1.0f;

    ::g_MeshesToDraw.push_back(pDwarf);

    cMeshObject* pDwarf2 = new cMeshObject();
    pDwarf2->meshFileName = "assets/models/Dungeon_models/Dead bodies, etc/SM_Prop_DeadBody_Dwarf_01.ply";
    pDwarf2->scale = 0.1f;
//    pDwarf2->bDoNotLight = true;
    pDwarf2->position.z = 60.0f;
    pDwarf2->position.x = 15.0f;
    pDwarf2->position.y = -10.0f;

    pDwarf2->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
    pDwarf2->textureBlendRatio[0] = 1.0f;

    pDwarf->shinniness = 100.0f;

    pDwarf2->bUseTextureAsColour = true;
    ::g_MeshesToDraw.push_back(pDwarf2);



    cMeshObject* pDeadBody_Nomad = new cMeshObject();
    pDeadBody_Nomad->meshFileName = "assets/models/Dungeon_models/Dead bodies, etc/SM_Prop_DeadBody_Nomad_02.ply";
    pDeadBody_Nomad->scale = 0.1f;
//    pDwarf->bDoNotLight = true;
    pDeadBody_Nomad->position.z = 60.0f;
    pDeadBody_Nomad->position.y = 10.0f;
    pDeadBody_Nomad->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
    pDeadBody_Nomad->textureBlendRatio[0] = 1.0f;

    pDeadBody_Nomad->specularHighlightColour = glm::vec3(0.0f, 0.0f, 0.0f);
    pDeadBody_Nomad->shinniness = 1.0f;

    ::g_MeshesToDraw.push_back(pDeadBody_Nomad);



    cMeshObject* pCrystals_Cluster_Large_01 = new cMeshObject();
    pCrystals_Cluster_Large_01->meshFileName = "assets/models/Dungeon_models/Crystals/SM_Env_Crystals_Cluster_Large_01.ply";
    pCrystals_Cluster_Large_01->scale = 0.05f;
//    pDwarf->bDoNotLight = true;
    pCrystals_Cluster_Large_01->position.z = 40.0f;
    pCrystals_Cluster_Large_01->position.x = 20.0f;
    pCrystals_Cluster_Large_01->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
    pCrystals_Cluster_Large_01->textureBlendRatio[0] = 1.0f;

    pCrystals_Cluster_Large_01->alphaTransparency = 0.5f;

//    pCrystals_Cluster_Large_01->specularHighlightColour = glm::vec3(0.0f, 0.0f, 0.0f);
    pCrystals_Cluster_Large_01->shinniness = 100.0f;

    ::g_MeshesToDraw.push_back(pCrystals_Cluster_Large_01);


    cMeshObject* pGround = new cMeshObject();
 //   pGround->meshFileName = "assets/models/Large_Terrain.ply";
    pGround->meshFileName = "assets/models/LargeFlatQuad.ply";
//    pGround->texture00Name = "SlothInSpace.bmp";

    pGround->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
//    pGround->textureNames[0] = "white-brick-wall-seamless-texture-free.bmp";
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
