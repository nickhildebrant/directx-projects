#include "Sampler.h"
#include "RendererErrorMacros.h"

Sampler::Sampler( Renderer& renderer )
{
	INFO_MANAGER( renderer );

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO( GetDevice( renderer )->CreateSamplerState( &samplerDesc, &pSampler ) );
}

void Sampler::Bind( Renderer& renderer ) noexcept
{
	GetContext( renderer )->PSSetSamplers( 0, 1, pSampler.GetAddressOf() );
}