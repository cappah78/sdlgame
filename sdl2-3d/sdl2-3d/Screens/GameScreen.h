#pragma once

#include "../Engine/Model/IScreen.h"
#include "../Engine/Model/IMouseListener.h"
#include "../Engine/Model/IKeyListener.h"

#include "../Engine/Graphics/PerspectiveCamera.h"
#include "../Engine/Utils/FPSCameraController.h"

#include "../Voxel/VoxelRenderer.h"
#include "../Voxel/VoxelWorld.h"
#include "../Voxel/WorldRenderer.h"

#include "../Engine/Graphics/GL/GLMesh.h"
#include "../Engine/Graphics/GL/GLShader.h"
#include "../Engine/Graphics/GLTextureManager.h"

/** Main screen containing executing the render and managing all entities */
class GameScreen : public IScreen, public IKeyListener
{
public:
	GameScreen();
	~GameScreen();
	void render(float deltaSec);
	void resize(int width, int height);
	virtual bool keyDown(SDL_Keysym key) override;
	virtual bool keyUp(SDL_Keysym key) override;

private:
	FPSCameraController m_cameraController;
	PerspectiveCamera m_camera;
	GLShader m_modelShader;
	GLMesh m_mesh;
	GLTextureManager m_textureManager;
	VoxelWorld m_world;
	WorldRenderer m_worldRenderer;
	VoxelRenderer m_voxelRenderer;
};
