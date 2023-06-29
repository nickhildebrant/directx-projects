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
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vertex.position);
			DirectX::XMStoreFloat3(&vertex.position, DirectX::XMVector3Transform(pos, matrix));
		}
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};