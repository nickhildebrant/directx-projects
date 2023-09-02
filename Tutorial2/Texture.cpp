#include "Texture.h"
#include "Surface.h"
#include "RendererErrorMacros.h"
#include "BindableCodex.h"

Texture::Texture( Renderer& renderer, const std::string& path, UINT slot) : slot(slot), path(path)
{
	INFO_MANAGER( renderer );

	const auto surface = Surface::FromFile( path );

	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = surface.GetWidth();
	textureDesc.Height = surface.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = surface.GetBufferPtr();
	sd.SysMemPitch = surface.GetWidth() * sizeof( Surface::Color );

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO( GetDevice( renderer )->CreateTexture2D( &textureDesc, &sd, &pTexture ) );

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO( GetDevice( renderer )->CreateShaderResourceView( pTexture.Get(), &srvDesc, &pTextureView ) );
}

void Texture::Bind( Renderer& renderer ) noexcept
{
	GetContext( renderer )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() );
}

std::shared_ptr<Bindable> Texture::Resolve( Renderer& renderer, const std::string& path, UINT slot )
{
	return Codex::Resolve<Texture>( renderer, path, slot );
}

std::string Texture::GenerateUID( const std::string& path, UINT slot )
{
	using namespace std::string_literals;
	return typeid( Texture ).name() + "#"s + path + "#" + std::to_string( slot );
}

std::string Texture::GetUID() const
{
	return GenerateUID( path, slot );
}