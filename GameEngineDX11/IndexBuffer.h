#pragma once
#include "Renderer.h"

class IndexBuffer {
public:
	IndexBuffer( Renderer& renderer, const std::vector<unsigned short>& indices );
	void Bind( Renderer& renderer );
	UINT GetCount() const;

protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};