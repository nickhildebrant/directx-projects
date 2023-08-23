#pragma once
#include "Bindable.h"

class Texture : public Bindable {
public:
	Texture( Renderer& renderer, const class Surface& s, unsigned int slot = 0 );
	void Bind( Renderer& renderer ) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;

private:
	unsigned int slot;
};