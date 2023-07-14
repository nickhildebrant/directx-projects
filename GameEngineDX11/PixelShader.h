#pragma once
#include "Bindable.h"

class PixelShader : public Bindable {
public:
	PixelShader( Renderer& renderer, const std::wstring& path );
	void Bind( Renderer& renderer ) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};
