#pragma once

struct SDL_Window;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
typedef void* SDL_GLContext;

class IGraphicsProvider;

/** Manages rendering related global variables and objects.*/
class Graphics
{
public:
	enum RenderMode { OPENGL, D3D };

	static void initialize(unsigned int screenWidth, unsigned int screenHeight, SDL_Window* const window);
	static void dispose();
	static void resizeScreen(unsigned int screenWidth, unsigned int screenHeight);
	static void setWindowTitle(const char* title);

	static void clear(float rCol, float gCol, float bCol, float aCol, bool clearColor = true, bool clearDepth = true);
	static void swap();

	static SDL_Window* getWindow();
	static unsigned int getScreenWidth();
	static unsigned int getScreenHeight();

	static inline RenderMode getRenderMode() { return s_renderMode; };
	static void setRenderMode(RenderMode mode) { s_renderMode = mode; }

	static IGraphicsProvider& getGraphicsProvider();

	static ID3D11Device* getDevice();
	static ID3D11DeviceContext* getDeviceContext();
	static SDL_GLContext getGLContext();


	static SDL_GLContext initializeGL();
	static void disposeGL(SDL_GLContext context);

private:

	static void initializeD3D();
	static void disposeD3D();

	static const RenderMode INITIAL_RENDERMODE = OPENGL;

	static RenderMode s_renderMode;
	static IGraphicsProvider* s_graphicsProvider;

	static SDL_Window* s_window;
	static unsigned int s_screenWidth;
	static unsigned int s_screenHeight;

	static SDL_GLContext s_glContext;

	static IDXGISwapChain* s_swapChain;
	static ID3D11Device* s_device;
	static ID3D11DeviceContext* s_deviceContext;
	static ID3D11RenderTargetView* s_renderTargetView;
	static ID3D11Texture2D* s_depthStencilBuffer;
	static ID3D11DepthStencilState* s_depthStencilState;
	static ID3D11DepthStencilView* s_depthStencilView;
	static ID3D11RasterizerState* s_rasterState;

	Graphics() {};
	Graphics(const Graphics& copy) {};
	~Graphics() {};
};