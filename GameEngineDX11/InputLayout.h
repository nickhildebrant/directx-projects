#pragma once
#include "Bindable.h"

class InputLayout : public Bindable {
public:
	InputLayout( Renderer& renderer, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDesc, ID3DBlob* blob )
	{
		renderer.GetDevice()->CreateInputLayout( inputElementDesc.data(), (UINT) inputElementDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &pInputLayout );
	}

	void Bind( Renderer& renderer ) override
	{
		renderer.GetDeviceContext()->IASetInputLayout( pInputLayout.Get() );
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};