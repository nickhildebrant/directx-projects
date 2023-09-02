#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Renderer& renderer, const std::string& path);
	void Bind(Renderer& renderer) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;

	static std::shared_ptr<VertexShader> Resolve( Renderer& renderer, const std::string& path );
	static std::string GenerateUID( const std::string& path );
	std::string GetUID() const override;

protected:
	std::string path;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};