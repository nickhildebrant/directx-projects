#pragma once
#include "Renderer.h"

class Bindable {

public:
	virtual void Bind(Renderer& renderer) noexcept = 0;
	virtual ~Bindable() = default;

	virtual std::string GetUID() const
	{
		assert( false );
		return "";
	}

protected:
	static ID3D11DeviceContext* GetContext(Renderer& renderer) noexcept;
	static ID3D11Device* GetDevice(Renderer& renderer) noexcept;
	static DxgiInfoManager& GetInfoManager(Renderer& renderer) noexcept;
};