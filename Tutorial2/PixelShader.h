#pragma once
#include "Bindable.h"

class PixelShader : public Bindable {
public:
	PixelShader(Renderer& renderer, const std::string& path);
	void Bind(Renderer& renderer) noexcept override;

	static std::shared_ptr<Bindable> Resolve( Renderer& renderer, const std::string& path );
	static std::string GenerateUID( const std::string& path );
	std::string GetUID() const override;

protected:
	std::string path;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};