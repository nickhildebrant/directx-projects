#pragma once
#include "Bindable.h"

class Texture : public Bindable {
public:
	Texture( Renderer& renderer, const std::string& path, UINT slot = 0 );
	void Bind( Renderer& renderer ) noexcept override;

	static std::shared_ptr<Bindable> Resolve( Renderer& renderer, const std::string& path, UINT slot = 0);
	static std::string GenerateUID( const std::string& path, UINT slot = 0);
	std::string GetUID() const override;

protected:
	std::string path;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;

private:
	unsigned int slot;
};