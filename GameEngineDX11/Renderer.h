#pragma once
#include <Windows.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>

class Renderer {
public:
	Renderer( HWND handle );

	void BeginFrame();			// Clear frame at start
	void EndFrame();			// Swap buffer

	void ClearBuffer( float r, float g, float b, float a );

	void DrawIndexed( UINT count );
	void SetProjection( DirectX::FXMMATRIX projection );
	DirectX::XMMATRIX GetProjection();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	void CreateDevice( HWND handle );
	void CreateRenderTarget();
	void CreateDepthStencil();

	DirectX::XMMATRIX m_projection;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;					// Used to create resources
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;				// Swap buffer at end of frame
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;	// Used to use resources and render

	// Render target
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// Stencil View
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
};