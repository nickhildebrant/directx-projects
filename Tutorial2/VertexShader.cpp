#include "VertexShader.h"
#include "RendererErrorMacros.h"

VertexShader::VertexShader(Renderer& renderer, const std::wstring& path)
{
	INFO_MANAGER(renderer);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &m_blob));
	GFX_THROW_INFO(GetDevice(renderer)->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_vertexShader));
}

void VertexShader::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->VSSetShader(m_vertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return m_blob.Get();
}