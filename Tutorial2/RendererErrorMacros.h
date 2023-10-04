#pragma once

#include "DirectXErrors.h"

#pragma comment(lib, "d3d11.lib") // compiler sets linker settings to allow D3D11CreateDeviceAndSwapChain
#pragma comment(lib, "D3DCompiler.lib")

// graphics exception macros
#define GFX_EXCEPT_NOINFO(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED( hr = (hrcall))) throw Renderer::GraphicsHrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define INFO_MANAGER(graphics) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((graphics))
#define GFX_EXCEPT(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Renderer::InfoException(__LINE__, __FILE__, v);}}
#else
#define INFO_MANAGER(graphics) HRESULT hr
#define GFX_EXCEPT(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif