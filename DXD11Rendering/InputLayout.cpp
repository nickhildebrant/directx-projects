#include "InputLayout.h"
#include "RendererErrorMacros.h"

InputLayout::InputLayout(Renderer& renderer, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* blob)
{
	INFO_MANAGER(renderer);

	GFX_THROW_INFO(GetDevice(renderer)->CreateInputLayout(layout.data(), (UINT)layout.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &m_inputLayout));
}

void InputLayout::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->IASetInputLayout(m_inputLayout.Get());
}