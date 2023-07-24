#pragma once
#include <Windows.h>
#include "ExceptionHandler.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "DxgiInfoManager.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

class Renderer {
	friend class Bindable;

public:
	class GraphicsException : public ExceptionHandler {
		using ExceptionHandler::ExceptionHandler;
	};

	class GraphicsHrException : public GraphicsException {
	public:
		GraphicsHrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;

		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;

	private:
		HRESULT m_hresult;
		std::string m_info;
	};

	class InfoException : public ExceptionHandler
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;

	private:
		std::string m_info;
	};

	class DeviceRemovedException : public GraphicsHrException {
		using GraphicsHrException::GraphicsHrException;
	public:
		const char* GetType() const noexcept override;

	private:
		std::string m_reason;
	};

public:
	Renderer(HWND handle);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer() = default;

	void EnableUI() noexcept;
	void DisableUI() noexcept;
	bool IsUIEnabled() const noexcept;

	void BeginFrame( float r, float g, float b, float a );	// Clear frame at start
	void EndFrame();								// Swap buffer

	void ClearBuffer(float r, float g, float b, float a);

	void DrawIndexed(UINT count);
	void SetProjection(DirectX::FXMMATRIX projection) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

private:
	bool uiEnabled = true;

	DirectX::XMMATRIX m_projection;

	void CreateDevice(HWND handle);
	void CreateRenderTarget();
	void CreateDepthStencil();

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;					// Used to create resources
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;				// Swap buffer at end of frame
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;	// Used to use resources and render

	// Render target
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// Stencil View
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
};