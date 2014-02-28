#include "GameScreen.h"

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../Game.h"
#include "../Voxel/VoxelRenderer.h"

static const float CAMERA_VERTICAL_FOV = 80.0f;
static const float CAMERA_NEAR = 0.5f;
static const float CAMERA_FAR = 280.0f;
static const glm::vec3 CAMERA_SPAWN_POS = glm::vec3(0, 2, -10);
static const glm::vec3 CAMERA_SPAWN_DIR = glm::vec3(0, 0, 1);

#define RENDER_MODEL 0

GameScreen::GameScreen()
	: m_camera(
		CAMERA_SPAWN_POS,
		CAMERA_SPAWN_DIR,
		float(Game::graphics.getScreenWidth()),
		float(Game::graphics.getScreenHeight()),
		CAMERA_VERTICAL_FOV,
		CAMERA_NEAR,
		CAMERA_FAR)
	, m_cameraController(m_camera, CAMERA_SPAWN_DIR)
	, m_modelShader("Assets/Shaders/modelshader.vert", NULL, "Assets/Shaders/modelshader.frag")
{
	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glEnable(GL_CULL_FACE);	
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/

#if RENDER_MODEL //work in progress testing
	///* initialize crytek sponza model rendering /*
	m_modelShader.begin();
	m_modelShader.setUniform1i("u_diffuseTex", 0);
	m_modelShader.setUniform1i("u_normalTex", 1);
	m_modelShader.setUniform1i("u_specularTex", 2);
	m_modelShader.setUniform1i("u_opacityTex", 3);
	m_modelShader.end();

	m_mesh.loadMesh("Assets/Models/crytek-sponza/sponza.obj", false);

	std::shared_ptr<GLMesh::ShaderAttributes> attribs(new GLMesh::ShaderAttributes(m_modelShader.getShaderID()));
	m_mesh.setShaderAttributes(attribs);
	//*/
#endif
}

void GameScreen::render(float deltaSec)
{
	Game::graphics.clear(0.4f, 0.7f, 1.0f, 1.0f);

	m_cameraController.update(deltaSec);
	m_camera.update();

#if !RENDER_MODEL
	m_world.update(deltaSec, m_camera);
	m_worldRenderer.render(m_world, m_camera);
#else

	///* renders crytek sponza model /*
	m_modelShader.begin();
	m_modelShader.setUniformMatrix4f("u_mvp", m_camera.m_combinedMatrix);
	m_modelShader.setUniformMatrix4f("u_transform", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.0f)), glm::vec3(0.1f)));
	m_mesh.render();
	m_modelShader.end();
	//*/

#endif //RENDER_MODEL

	Game::graphics.swap();
}

GameScreen::~GameScreen() 
{

}

void GameScreen::resize(int width, int height) 
{
	glViewport(0, 0, width, height);
	m_camera.resize(width, height);
}

static int camSpeedTicks = 0;
bool GameScreen::keyDown(SDL_Keysym key)
{
	if (key.sym == SDLK_r)
	{
		//just some quick way to control speed
		camSpeedTicks++;
		m_cameraController.setCameraSpeed(10.0f * ((camSpeedTicks % 7) + 1));
	}

	if (key.sym == SDLK_t)
	{

	}

	if (key.sym == SDLK_f)
	{

	}

	if (key.sym == SDLK_PERIOD)
	{
		m_camera.setNearFar(m_camera.m_near, m_camera.m_far + 10.0f);
	}

	if (key.sym == SDLK_COMMA)
	{
		m_camera.setNearFar(m_camera.m_near, m_camera.m_far - 10.0f);
	}

	if (key.sym == SDLK_F1)
	{
		printf("Controls: \n"
			"WASD, Shift, Space to fly \n"
			"R to toggle different speeds \n"
			"Period to increase render distance \n"
			"Comma to decrease render distance \n"
			"F to trigger a block update at location \n"
			"T to place a light at location(wip)\n");
	}


	return false;
}
bool GameScreen::keyUp(SDL_Keysym key)
{
	return false;
}

