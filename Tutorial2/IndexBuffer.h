#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(Renderer& renderer, const std::vector<unsigned short>& indices);
	IndexBuffer( Renderer& renderer, std::string tag, const std::vector<unsigned short>& indicies );

	void Bind(Renderer& renderer) noexcept override;
	UINT GetCount() const noexcept;

	static std::shared_ptr<Bindable> Resolve( Renderer& renderer, const std::string& tag, const std::vector<unsigned short>& indices );

	template<typename...Ignore>
	static std::string GenerateUID( const std::string& tag, Ignore&&...ignore )
	{
		return GenerateUID_( tag );
	}

	std::string GetUID() const noexcept override;

protected:
	std::string tag;

	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

private:
	static std::string GenerateUID_( const std::string& tag );
};