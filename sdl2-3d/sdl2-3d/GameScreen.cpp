#include "GameScreen.h"

#include <iostream>
#include <stdio.h>

#define TERRAIN_VERTEX_SHADER "terrain.vert"
#define TERRAIN_FRAGMENT_SHADER "terrain.frag"

#define GEOMETRY_PASS_VERTEX_SHADER "geometrypass.vert"
#define GEOMETRY_PASS_FRAGMENT_SHADER "geometrypass.frag"

GameScreen::GameScreen() 
	: batch(4)
	, camera(glm::vec3(0, 10, 0), glm::vec3(0, 0, -1))
	, cameraController(camera)
	, heightMap(23, 512, 512)
	, terrainShader(TERRAIN_VERTEX_SHADER, TERRAIN_FRAGMENT_SHADER)
	, geomPassShader(GEOMETRY_PASS_VERTEX_SHADER, GEOMETRY_PASS_FRAGMENT_SHADER)
	, lightManager(terrainShader.shaderId)
	, sphere("box.obj")
{
	SDLGame::registerKeyListener(&cameraController);
	SDLGame::registerMouseListener(&mouseLookSystem);
	
	heightMap.addPerlinNoise(30.0f);
	heightMap.addPerlinNoise(20.0f);
	heightMap.addPerlinNoise(10.0f);
	heightMap.addPerlinNoise(5.0f);
	heightMap.addPerlinNoise(1.0f);

	heightMap.perturb(250.0f, 8.0f);
	for(int i = 0; i < 5; i++)
		heightMap.erode(16.0f);
	heightMap.smoothen();
	heightMap.smoothen();


	terrain = new Terrain2(heightMap, 10, 100);
	SDLGame::registerKeyListener(terrain);

	texture = new Texture("grass.png");

	world.registerSystem(movementSystem);
	world.registerSystem(mouseLookSystem);

	EntityFactory::createPlayer(world, 1, 1, 0);
}

GameScreen::~GameScreen() 
{
	delete texture;
	delete texture2;
	delete texture3;
	delete region;

	delete terrain;
}

void GameScreen::renderSceneCB()
{
	camera.update();
	gbuffer.startFrame();
	geometryPassDS();
	lightPassDS();
}

void GameScreen::geometryPassDS()
{
	geomPassShader.use(camera);
	gbuffer.bindForGeomPass();
	
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	texture->bind();
	terrain->render();

	glDepthMask(GL_FALSE);
}

void GameScreen::stencilPassDS()
{
	/*
	m_nullTech.Enable();

	// Disable color/depth write and enable stencil
	m_gbuffer.BindForStencilPass();
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	// We need the stencil test to be enabled but we want it
	// to succeed always. Only the depth test matters.
	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	Pipeline p;
	p.WorldPos(m_pointLight[PointLightIndex].Position);
	float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
	p.Scale(BBoxScale, BBoxScale, BBoxScale);		
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_nullTech.SetWVP(p.GetWVPTrans());
	m_bsphere.Render();  
	*/
}

void GameScreen::lightPassDS()
{

}

void GameScreen::render(float deltaSec) 
{
	world.loopStart();
	world.setDelta(deltaSec);
	world.update();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraController.update(deltaSec);
	camera.lookAtDir(mouseLookSystem.getLookDir());
	camera.update();
	
	terrainShader.use(camera);
	lightManager.update(camera);

	texture->bind();
	terrain->render();
	sphere.render();
	
	SDLGame::swap();
}

void GameScreen::resize(int width, int height) 
{
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
	camera.resize(width, height);
}

