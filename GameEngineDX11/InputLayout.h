#pragma once
#include "Bindable.h"

class InputLayout : public Bindable {
public:
	InputLayout( Renderer& renderer, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDesc, ID3DBlob* blob );

	void Bind( Renderer& renderer ) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};
