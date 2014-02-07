#include "GameScreen.h"

#include <glm\glm.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>


#include <SDL_syswm.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

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
	, m_renderMode(RenderMode::OPENGL)
{
//	initializeD3D();

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

#define RENDER_WITH_OPENGL 1;
void GameScreen::render(float deltaSec)
{
	switch (m_renderMode)
	{
	case OPENGL:
		renderOpenGL(deltaSec);
		break;
	case D3D:
		renderD3D(deltaSec);
		break;
	default:
		assert(false);
		break;
	}
}

void GameScreen::renderD3D(float deltaSec)
{
	/*
	m_deviceContext->ClearRenderTargetView(m_backBuffer, &glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)[0]);
	
	unsigned int stride = sizeof(glm::vec3);
	unsigned int offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->Draw(3, 0);
	
	m_swapChain->Present(0, 0);
	*/
}

void GameScreen::renderOpenGL(float deltaSec)
{
	glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_cameraController.update(deltaSec);
	m_camera.update();

	m_world.update(deltaSec, m_camera);
	m_worldRenderer.render(m_world, m_camera);
	
	/* renders crytek sponza model /*
	m_modelShader.begin();
	m_modelShader.setUniformMatrix4f("u_mvp", m_camera.m_combinedMatrix);
	m_modelShader.setUniformMatrix4f("u_transform", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 0, -20)), glm::vec3(0.1f, 0.1f, 0.1f)));
	m_mesh.render();
	m_modelShader.end();
	*/
	Game::graphics.swap();
}

void GameScreen::initializeD3D()
{
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);
	SDL_GetWindowWMInfo(Game::graphics.getWindow(), &windowInfo);
	const HWND& windowsWindowHandle = windowInfo.info.win.window;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowsWindowHandle;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext);

	ID3D11Texture2D* backBufferSurface;

	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &backBufferSurface);
	m_device->CreateRenderTargetView(backBufferSurface, NULL, &m_backBuffer);
	backBufferSurface->Release();
	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 1600;
	viewport.Height = 900;
	m_deviceContext->RSSetViewports(1, &viewport);

	glm::vec3 vertices[] =
	{
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.45f, -0.5f, 0.0f),
		glm::vec3(-0.45f, -0.5f, 0.0f)
	};

	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	m_device->CreateBuffer(&bufferDescription, NULL, &m_vertexBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	m_deviceContext->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubresource);
	memcpy(mappedSubresource.pData, vertices, sizeof(vertices));
	m_deviceContext->Unmap(m_vertexBuffer, NULL);

	/*
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	const char* const shaderPath = "Assets/Shaders/d3dtest.hlsl";
	D3D10CompileShader(shaderPath, strlen(shaderPath), "VShader", NULL, NULL, "d3dtest.hlsl", "vs_5_0", 0, &vertexShaderBlob, NULL);
	D3D10CompileShader(shaderPath, strlen(shaderPath), "PShader", NULL, NULL, "d3dtest.hlsl", "ps_5_0", 0, &pixelShaderBlob, NULL);
	
	m_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &m_vertexShader);
	m_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &m_pixelShader);

	ID3D11InputLayout* inputLayout;
	D3D11_INPUT_ELEMENT_DESC inputElementDescription[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_device->CreateInputLayout(inputElementDescription, 1, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);

	m_deviceContext->VSSetShader(m_vertexShader, 0, 0);
	m_deviceContext->PSSetShader(m_pixelShader, 0, 0);
	m_deviceContext->IASetInputLayout(inputLayout);
	*/
}

void GameScreen::disposeD3D()
{
	m_swapChain->Release();
	m_backBuffer->Release();
	m_device->Release();
	m_deviceContext->Release();
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
		switch (m_renderMode)
		{
		case OPENGL:
			m_renderMode = D3D;
			break;
		case D3D:
			m_renderMode = OPENGL;
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

