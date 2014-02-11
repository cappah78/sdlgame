#pragma once

struct SDL_Window;
struct ID3D11Device;
struct ID3D11DeviceContext;
class IGraphicsProvider;

/** Manages rendering related global variables and objects.*/
class Graphics
{
public:
	enum RenderMode { OPENGL, D3D };
private:
	static const RenderMode INITIAL_RENDERMODE = OPENGL;
public:

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
	static void initializeD3D();
	static void disposeD3D();
	static ID3D11Device* getDevice();
	static ID3D11DeviceContext* getDeviceContext();

private:
	static RenderMode s_renderMode;
	static IGraphicsProvider* s_graphicsProvider;

	Graphics() {};
	Graphics(const Graphics& copy) {};
	~Graphics() {};
};