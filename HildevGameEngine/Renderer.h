#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

class Renderer {
	//friend class Bindable;

public:
	Renderer( HWND handle, int width, int height );
	Renderer( const Renderer& ) = delete;
	Renderer& operator=( const Renderer& ) = delete;
	~Renderer();

	void EnableUI();
	void DisableUI();
	bool IsUIEnabled() const;

	void BeginFrame( float r, float g, float b, float a );	// Clear frame at start
	void EndFrame();										// Swap buffer

	void ClearBuffer( float r, float g, float b, float a );

	void DrawIndexed( UINT count );

	void SetCameraView( DirectX::FXMMATRIX view );
	DirectX::XMMATRIX GetView() const;

	void SetProjection( DirectX::FXMMATRIX projection );
	DirectX::XMMATRIX GetProjection() const;

private:
	int m_width, m_height;

	bool uiEnabled = true;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;

	void CreateDevice( HWND handle );
	void CreateRenderTarget();
	void CreateDepthStencil();

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;					// Used to create resources
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;				// Swap buffer at end of frame
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;	// Used to use resources and render

	// Render target
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;

	// Stencil View
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencil;
};