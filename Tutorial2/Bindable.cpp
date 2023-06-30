#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Renderer& renderer) noexcept
{
	return renderer.m_deviceContext.Get();
}

ID3D11Device* Bindable::GetDevice(Renderer& renderer) noexcept
{
	return renderer.m_device.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Renderer& renderer) noexcept
{
#ifndef NDEBUG
	return renderer.infoManager;
#else
	throw std::logic_error("Tried to access renderer.infoManager in Release config");
#endif
}