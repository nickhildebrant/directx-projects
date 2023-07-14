#include "InputLayout.h"

InputLayout::InputLayout( Renderer& renderer, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDesc, ID3DBlob* blob )
{
	renderer.GetDevice()->CreateInputLayout( inputElementDesc.data(), (UINT) inputElementDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &pInputLayout );
}

void InputLayout::Bind( Renderer& renderer )
{
	renderer.GetDeviceContext()->IASetInputLayout( pInputLayout.Get() );
}