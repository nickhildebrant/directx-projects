#pragma once
#include "Renderer.h"

class Cube {
public:
	Cube( Renderer& renderer );
	~Cube();
	void Draw( Renderer& renderer );

private:
	void CreateMesh( Renderer& renderer );
	void CreateShaders( Renderer& renderer );

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
	ID3D11Buffer* m_cBuffer = nullptr;
};