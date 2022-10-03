#pragma once
#include <Windows.h>
#include "ExceptionHandler.h"
#include <d3d11.h>
#include <vector>
#include "DxgiInfoManager.h"

class Renderer {
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
	~Renderer();

	void BeginFrame();			// Clear frame at start
	void EndFrame();			// Swap buffer

	void ClearBuffer(float r, float g, float b);

private:
	void CreateDevice(HWND handle);
	void CreateRenderTarget();

#if defined(_DEBUG)
	DxgiInfoManager infoManager;
#endif
	ID3D11Device* m_device = nullptr;				// Used to create resources
	IDXGISwapChain* m_swapChain = nullptr;			// Swap buffer at end of frame
	ID3D11DeviceContext* m_deviceContext = nullptr;	// Used to use resources and render

	// Render target
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	//D3D11_Resource_desc m_backBufferDesc;
};