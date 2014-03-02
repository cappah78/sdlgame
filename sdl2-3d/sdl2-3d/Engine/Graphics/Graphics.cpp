#include "Graphics.h"

#include "GL\GLGraphicsProvider.h"

#include <assert.h>

#include <SDL.h>
#include <SDL_syswm.h>
#include <gl\glew.h>

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dcompiler.lib")

Graphics::RenderMode Graphics::s_renderMode = Graphics::INITIAL_RENDERMODE;
IGraphicsProvider* Graphics::s_graphicsProvider = NULL;

SDL_Window* Graphics::s_window = NULL;
unsigned int Graphics::s_screenWidth = 0;
unsigned int Graphics::s_screenHeight = 0;

SDL_GLContext Graphics::s_glContext = NULL;

IDXGISwapChain* Graphics::s_swapChain = NULL;
ID3D11Device* Graphics::s_device = NULL;
ID3D11DeviceContext* Graphics::s_deviceContext = NULL;
ID3D11RenderTargetView* Graphics::s_renderTargetView = NULL;
ID3D11Texture2D* Graphics::s_depthStencilBuffer = NULL;
ID3D11DepthStencilState* Graphics::s_depthStencilState = NULL;
ID3D11DepthStencilView* Graphics::s_depthStencilView = NULL;
ID3D11RasterizerState* Graphics::s_rasterState = NULL;

void Graphics::initialize(unsigned int screenWidth, unsigned int screenHeight, SDL_Window* const window)
{
	s_window = window;
	s_screenWidth = screenWidth;
	s_screenHeight = screenHeight;

	s_graphicsProvider = new GLGraphicsProvider();

	switch (s_renderMode)
	{
	case OPENGL:
		s_glContext = initializeGL();
		break;
	case D3D:
		initializeD3D();
		break;
	}
}

void Graphics::dispose()
{
	disposeGL(s_glContext);
	disposeD3D();
}

IGraphicsProvider& Graphics::getGraphicsProvider()
{
	return *s_graphicsProvider;
}

void Graphics::clear(float rCol, float gCol, float bCol, float aCol, bool clearColor, bool clearDepth)
{
	unsigned int clearFlags = 0;
	float colorClearVal[] = { rCol, gCol, bCol, aCol };
	const float depthClearVal = 1.0f;

	switch (s_renderMode)
	{
	case OPENGL:
		if (clearColor)
			glClearBufferfv(GL_COLOR, 0, colorClearVal);
		if (clearDepth)
			glClearBufferfv(GL_DEPTH, 0, &depthClearVal);
		break;
	case D3D:
		if (clearColor)
			s_deviceContext->ClearRenderTargetView(s_renderTargetView, colorClearVal);
		if (clearDepth)
			s_deviceContext->ClearDepthStencilView(s_depthStencilView, D3D11_CLEAR_DEPTH, depthClearVal, 0);
		break;
	}
}

void Graphics::swap()
{
	switch (s_renderMode)
	{
	case OPENGL:
		SDL_GL_SwapWindow(s_window);
		break;
	case D3D:
		s_swapChain->Present(0, 0);	//swap without vsync
		break;
	}
}

void Graphics::resizeScreen(unsigned int screenWidth, unsigned int screenHeight)
{
	s_screenWidth = screenWidth;
	s_screenHeight = screenHeight;
}

void Graphics::setWindowTitle(const char* title)
{
	SDL_SetWindowTitle(s_window, title);
}

SDL_Window* Graphics::getWindow()
{
	return s_window;
}

unsigned int Graphics::getScreenWidth()
{
	return s_screenWidth;
}

unsigned int Graphics::getScreenHeight()
{
	return s_screenHeight;
}

SDL_GLContext Graphics::initializeGL()
{
	SDL_GLContext context = SDL_GL_CreateContext(s_window);

	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error at initializeGL() : %s \n", error);
		SDL_ClearError();
	}

	SDL_GL_SetSwapInterval(0);	//1 is vsync 0 is uncapped

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	else
	{	
		//clear invalid enum bullshit error
		for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError());
	}
	return context;
}

void Graphics::disposeGL(SDL_GLContext context)
{
	SDL_GL_DeleteContext(context);
}

void Graphics::initializeD3D()
{
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);
	SDL_GetWindowWMInfo(s_window, &windowInfo);
	const HWND& windowsWindowHandle = windowInfo.info.win.window;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;                                  // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // use 32-bit color
	swapChainDesc.BufferDesc.Width = s_screenWidth;					// set the back buffer width
	swapChainDesc.BufferDesc.Height = s_screenHeight;				// set the back buffer height
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how swap chain is to be used
	swapChainDesc.OutputWindow = windowsWindowHandle;               // the window to be used
	swapChainDesc.SampleDesc.Count = 1;                             // how many multisamples
	swapChainDesc.Windowed = TRUE;                                  // windowed/full-screen mode
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // allow full-screen switching

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&s_swapChain,
		&s_device,
		NULL,
		&s_deviceContext);
	assert(SUCCEEDED(result));

	ID3D11Texture2D* backBufferPtr;
	result = s_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &backBufferPtr);
	assert(SUCCEEDED(result));
	result = s_device->CreateRenderTargetView(backBufferPtr, NULL, &s_renderTargetView);
	assert(SUCCEEDED(result));
	backBufferPtr->Release();

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = s_screenWidth;
	depthBufferDesc.Height = s_screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	result = s_device->CreateTexture2D(&depthBufferDesc, NULL, &s_depthStencilBuffer);
	assert(SUCCEEDED(result));

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	result = s_device->CreateDepthStencilState(&depthStencilDesc, &s_depthStencilState);
	assert(SUCCEEDED(result));

	s_deviceContext->OMSetDepthStencilState(s_depthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = s_device->CreateDepthStencilView(s_depthStencilBuffer, &depthStencilViewDesc, &s_depthStencilView);
	assert(SUCCEEDED(result));
	s_deviceContext->OMSetRenderTargets(1, &s_renderTargetView, s_depthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	result = s_device->CreateRasterizerState(&rasterDesc, &s_rasterState);
	assert(SUCCEEDED(result));

	s_deviceContext->RSSetState(s_rasterState);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = (float) s_screenWidth;
	viewport.Height = (float) s_screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	s_deviceContext->RSSetViewports(1, &viewport);
}

void Graphics::disposeD3D()
{
	s_swapChain->Release();
	s_renderTargetView->Release();
	s_device->Release();
	s_deviceContext->Release();
	s_depthStencilBuffer->Release();
	s_depthStencilState->Release();
	s_depthStencilView->Release();
	s_rasterState->Release();
}

ID3D11Device* Graphics::getDevice()
{
	return s_device;
}

ID3D11DeviceContext* Graphics::getDeviceContext()
{
	return s_deviceContext;
}

SDL_GLContext Graphics::getGLContext()
{
	return s_glContext;
}
