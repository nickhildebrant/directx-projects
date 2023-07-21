#include "TextureMgr.h"

TextureMgr::TextureMgr() : md3dDevice( 0 )
{
}

TextureMgr::~TextureMgr()
{
	for ( auto it = mTextureSRV.begin(); it != mTextureSRV.end(); ++it )
	{
		ReleaseCOM( it->second );
	}

	mTextureSRV.clear();
}

void TextureMgr::Init( ID3D11Device* device )
{
	md3dDevice = device;
}

ID3D11ShaderResourceView* TextureMgr::CreateTexture( std::wstring filename )
{
	ID3D11ShaderResourceView* srv = 0;

	// Does it already exist?
	if ( mTextureSRV.find( filename ) != mTextureSRV.end() )
	{
		srv = mTextureSRV[filename];
	}
	else
	{
		//md3dDevice, filename.c_str(), 0, 0, & srv, 0 )
		HR( DirectX::CreateDDSTextureFromFileEx( md3dDevice, filename.c_str(), 8, D3D11_USAGE_DEFAULT, 0, 0, 0, 0, nullptr, &srv, nullptr ) );

		mTextureSRV[filename] = srv;
	}

	return srv;
}
