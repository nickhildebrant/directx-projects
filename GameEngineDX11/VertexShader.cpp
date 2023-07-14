#include "VertexShader.h"

VertexShader::VertexShader( Renderer& renderer, const std::wstring& path )
{
	D3DReadFileToBlob( path.c_str(), &pBlob );
	renderer.GetDevice()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader );
}

void VertexShader::Bind( Renderer& renderer )
{
	renderer.GetDeviceContext()->VSSetShader( pVertexShader.Get(), nullptr, 0u );
}

ID3DBlob* VertexShader::GetBytecode() const { return pBlob.Get(); }