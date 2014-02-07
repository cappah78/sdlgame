#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include "../Engine/Model/IScreen.h"
#include "../Engine/Model/IMouseListener.h"
#include "../Engine/Model/IKeyListener.h"

#include "../Engine/Graphics/PerspectiveCamera.h"
#include "../Engine/Utils/FPSCameraController.h"

#include "../Voxel/VoxelRenderer.h"
#include "../Voxel/VoxelWorld.h"
#include "../Voxel/WorldRenderer.h"
#include "../Voxel/Deferred/DeferredWorldRenderer.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11Buffer;
class VoxelCache;
class TextureArray;

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
	Shader m_modelShader;
	Mesh m_mesh;
	TextureManager m_textureManager;
	VoxelWorld m_world;
	WorldRenderer m_worldRenderer;
	VoxelRenderer m_voxelRenderer;
	TextureArray* m_tileSet;

	//tmp d3d stuffs
	enum RenderMode { OPENGL, D3D };
	void initializeD3D();	//tmp testing stuffs
	void disposeD3D();
	
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_backBuffer;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;

	void renderOpenGL(float deltaSec);
	void renderD3D(float deltaSec);
	RenderMode m_renderMode;
	///
};

#endif //GAME_SCREEN_H_
