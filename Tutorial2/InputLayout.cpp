#include "InputLayout.h"
#include "RendererErrorMacros.h"
#include "BindableCodex.h"

InputLayout::InputLayout(Renderer& renderer, VertexHandler::VertexLayout layout_in, ID3DBlob* blob) : layout( std::move(layout_in) )
{
	INFO_MANAGER(renderer);

	const std::vector<D3D11_INPUT_ELEMENT_DESC> d3dLayout = layout.GetD3DLayout();
	GFX_THROW_INFO(GetDevice(renderer)->CreateInputLayout( d3dLayout.data(), (UINT)d3dLayout.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &m_inputLayout));
}

void InputLayout::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->IASetInputLayout(m_inputLayout.Get());
}

std::shared_ptr<InputLayout> InputLayout::Resolve( Renderer& renderer, const VertexHandler::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode )
{
	return Codex::Resolve<InputLayout>( renderer, layout, pVertexShaderBytecode );
}

std::string InputLayout::GenerateUID( const VertexHandler::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode )
{
	using namespace std::string_literals;
	return typeid( InputLayout ).name() + "#"s + layout.GetCode();
}

std::string InputLayout::GetUID() const noexcept
{
	return GenerateUID( layout );
}