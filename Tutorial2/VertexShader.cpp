#include "VertexShader.h"
#include "RendererErrorMacros.h"
#include "BindableCodex.h"

VertexShader::VertexShader(Renderer& renderer, const std::string& path) : path(path)
{
	INFO_MANAGER(renderer);

	GFX_THROW_INFO( D3DReadFileToBlob( std::wstring{path.begin(), path.end()}.c_str(), & pBlob ) );
	GFX_THROW_INFO( GetDevice( renderer )->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader ) );
}

void VertexShader::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->VSSetShader( pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBlob.Get();
}

std::shared_ptr<VertexShader> VertexShader::Resolve( Renderer& renderer, const std::string& path )
{
	return Codex::Resolve<VertexShader>( renderer, path );
}

std::string VertexShader::GenerateUID( const std::string& path )
{
	using namespace std::string_literals;
	return typeid( VertexShader ).name() + "#"s + path;
}

std::string VertexShader::GetUID() const
{
	return GenerateUID( path );
}