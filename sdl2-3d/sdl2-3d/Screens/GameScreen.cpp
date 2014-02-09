#include "GameScreen.h"

#include <glm\glm.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include <SDL_syswm.h>
#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "../Engine/Utils/FileReader.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Game.h"
#include "../Voxel/VoxelRenderer.h"

#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Color8888.h"
#include "../Engine/Graphics/GL/Mesh.h"

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
	, m_textureManager()
	, m_world(m_textureManager)
	, m_modelShader("Assets/Shaders/modelshader.vert", NULL, "Assets/Shaders/modelshader.frag")
{
	initializeD3D();

	Game::input.registerKeyListener(&m_cameraController);
	Game::input.registerMouseListener(&m_cameraController);
	Game::input.registerKeyListener(this);

	glEnable(GL_DEPTH_TEST);

#if RENDER_MODEL //work in progress testing
	///* initialize crytek sponza model rendering /*
	m_modelShader.begin();
	m_modelShader.setUniform1i("u_diffuseTex", 0);
	m_modelShader.setUniform1i("u_normalTex", 1);
	m_modelShader.setUniform1i("u_specularTex", 2);
	m_modelShader.setUniform1i("u_opacityTex", 3);
	m_modelShader.end();

	m_mesh.loadMesh("Assets/Models/crysponza_bubbles/sponza.obj", m_textureManager);
	std::shared_ptr<Mesh::ShaderAttributes> attribs(new Mesh::ShaderAttributes(m_modelShader.getShaderID()));
	m_mesh.setShaderAttributes(attribs);
	//*/
#endif
}

void GameScreen::render(float deltaSec)
{
	switch (Game::graphics.getRenderMode())
	{
	case Graphics::RenderMode::OPENGL:
		renderOpenGL(deltaSec);
		break;
	case Graphics::RenderMode::D3D:
		renderD3D(deltaSec);
		break;
	default:
		assert(false);
		break;
	}
}

struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
};

void GameScreen::renderD3D(float deltaSec)
{
	Game::graphics.clear(1.0f, 0.7f, 1.0f, 1.0f);
	Game::graphics.swap();
}

void GameScreen::renderOpenGL(float deltaSec)
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
	m_modelShader.setUniformMatrix4f("u_transform", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 0, -20)), glm::vec3(0.1f, 0.1f, 0.1f)));
	m_mesh.render();
	m_modelShader.end();
	//*/
#endif //RENDER_MODEL
	Game::graphics.swap();
}

void GameScreen::initializeD3D()
{
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	std::string fileData = FileReader::readStringFromFile("Assets/Shaders/d3dtest.hlsl");
	const char* cStr = fileData.c_str();

	static const int MAX_OPTIMIZE_FLAG = (1 << 15);
	bool compiledVert = SUCCEEDED(D3DCompile(cStr, fileData.size(), "d3dtest.hlsl", NULL, NULL, "VShader", "vs_5_0", MAX_OPTIMIZE_FLAG, 0, &vertexShaderBlob, NULL));
	bool compiledPix = SUCCEEDED(D3DCompile(cStr, fileData.size(), "d3dtest.hlsl", NULL, NULL, "PShader", "ps_5_0", MAX_OPTIMIZE_FLAG, 0, &pixelShaderBlob, NULL));
	printf("compiled %i %i \n", compiledVert, compiledPix);
	
	auto device = Game::graphics.getDevice();
	auto deviceContext = Game::graphics.getDeviceContext();

	device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &m_vertexShader);
	device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &m_pixelShader);

	deviceContext->VSSetShader(m_vertexShader, 0, 0);
	deviceContext->PSSetShader(m_pixelShader, 0, 0);

	ID3D11InputLayout* inputLayout;
	D3D11_INPUT_ELEMENT_DESC inputElementDescription[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(inputElementDescription, 2, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
	deviceContext->IASetInputLayout(inputLayout);

	{
		Vertex vertices[] =
		{
			{ glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(1.0, 0.0, 0.0, 1.0) },
			{ glm::vec3(0.45f, 0.0f, 0.0f), glm::vec4(0.0, 1.0, 0.0, 1.0) },
			{ glm::vec3(-0.45f, 0.0f, 0.0f), glm::vec4(0.0, 0.0, 1.0, 1.0) },
			{ glm::vec3(0.0f, -0.5f, 0.0f), glm::vec4(1.0, 1.0, 1.0, 1.0) }
		};

		D3D11_BUFFER_DESC bufferDescription;
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.ByteWidth = sizeof(vertices);
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device->CreateBuffer(&bufferDescription, NULL, &m_vertexBuffer);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		deviceContext->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubresource);
		memcpy(mappedSubresource.pData, vertices, sizeof(vertices));
		deviceContext->Unmap(m_vertexBuffer, NULL);
	}
	{
		unsigned int indices[] =
		{
			0, 1, 2, 1, 3, 2
		};

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexBufferDesc.ByteWidth = sizeof(indices);
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device->CreateBuffer(&indexBufferDesc, NULL, &m_indiceBuffer);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		deviceContext->Map(m_indiceBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubresource);
		memcpy(mappedSubresource.pData, indices, sizeof(indices));
		deviceContext->Unmap(m_indiceBuffer, NULL);
	}
}

void GameScreen::disposeD3D()
{
	m_pixelShader->Release();
	m_vertexShader->Release();
}

GameScreen::~GameScreen() 
{
	disposeD3D();
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
		switch (Game::graphics.getRenderMode())
		{
		case Graphics::RenderMode::OPENGL:

			Game::graphics.setRenderMode(Graphics::RenderMode::D3D);
			break;
		case Graphics::RenderMode::D3D:
			Game::graphics.setRenderMode(Graphics::RenderMode::OPENGL);
			break;
		default:
			assert(false);
			break;
		}
	}

	if (key.sym == SDLK_f)
	{
		//m_world.doBlockUpdate(glm::ivec3(glm::round(m_camera.m_position)));
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

