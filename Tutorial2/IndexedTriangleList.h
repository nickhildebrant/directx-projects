#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class IndexedTriangleList {
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> vertices_in, std::vector<unsigned short> indices_in)
		: vertices(std::move(vertices_in)), indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& vertex : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat4(&vertex.position);
			DirectX::XMStoreFloat4(&vertex.position, DirectX::XMVector4Transform(pos, matrix));
		}
	}

	void SetNormalsIndependentFlat() noexcept
	{
		assert( indices.size() % 3 == 0 && indices.size() > 0 );

		for ( size_t i = 0; i < indices.size(); i += 3 )
		{
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];

			const auto p0 = DirectX::XMLoadFloat4( &v0.position );
			const auto p1 = DirectX::XMLoadFloat4( &v1.position );
			const auto p2 = DirectX::XMLoadFloat4( &v2.position );

			const auto cross = DirectX::XMVector3Cross( DirectX::XMVectorSubtract( p1, p0 ), DirectX::XMVectorSubtract( p2, p0 ) );
			const auto normal = DirectX::XMVector4Normalize( cross );

			DirectX::XMStoreFloat4( &v0.normal, normal );
			DirectX::XMStoreFloat4( &v1.normal, normal );
			DirectX::XMStoreFloat4( &v2.normal, normal );
		}
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};