#include "PixelShader.h"
#include "RendererErrorMacros.h"
#include "BindableCodex.h"

PixelShader::PixelShader(Renderer& renderer, const std::string& path) : path(path)
{
	INFO_MANAGER(renderer);

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	GFX_THROW_INFO( D3DReadFileToBlob( std::wstring{path.begin(), path.end()}.c_str(), & blob));
	GFX_THROW_INFO(GetDevice(renderer)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixelShader));
}

void PixelShader::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->PSSetShader(m_pixelShader.Get(), nullptr, 0u);
}

std::shared_ptr<PixelShader> PixelShader::Resolve( Renderer& renderer, const std::string& path )
{
	return Codex::Resolve<PixelShader>( renderer, path );
}

std::string PixelShader::GenerateUID( const std::string& path )
{
	using namespace std::string_literals;
	return typeid( PixelShader ).name() + "#"s + path;
}

std::string PixelShader::GetUID() const
{
	return GenerateUID( path );
}