#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

const float PI = 3.14159265f;

class Cone {
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longitudeDivisions)
	{
		assert(longitudeDivisions >= 3);

		const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longitudeDivisions;

		// base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longitudeDivisions; iLong++)
		{
			vertices.emplace_back();
			auto vec = DirectX::XMVector4Transform(base, DirectX::XMMatrixRotationZ(longitudeAngle * iLong));
			DirectX::XMStoreFloat4(&vertices.back().position, vec);
		}

		// the center
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,-1.0f,1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);

		// the tip :darkness:
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,1.0f,1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);


		// base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longitudeDivisions; iLong++)
		{
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % longitudeDivisions);
			indices.push_back(iLong);
		}

		// cone indices
		for (unsigned short iLong = 0; iLong < longitudeDivisions; iLong++)
		{
			indices.push_back(iLong);
			indices.push_back((iLong + 1) % longitudeDivisions);
			indices.push_back(iTip);
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentFaces( int longDiv )
	{
		assert( longDiv >= 3 );

		const auto base = DirectX::XMVectorSet( 1.0f, 0.0f, -1.0f, 0.0f );
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;

		// cone vertices
		const auto iCone = (unsigned short) vertices.size();
		for ( int iLong = 0; iLong < longDiv; iLong++ )
		{
			const float thetas[] = {
				longitudeAngle * iLong,
				longitudeAngle * ( ( ( iLong + 1 ) == longDiv ) ? 0 : ( iLong + 1 ) )
			};

			vertices.emplace_back();
			vertices.back().position = { 0.0f,0.0f,1.0f,1.0f };

			for ( auto theta : thetas )
			{
				vertices.emplace_back();
				const auto vec = DirectX::XMVector4Transform( base, DirectX::XMMatrixRotationZ( theta ) );
				DirectX::XMStoreFloat4( &vertices.back().position, vec );
			}
		}

		// base vertices
		const auto iBaseCenter = (unsigned short) vertices.size();
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,-1.0f,1.0f };

		const auto iBaseEdge = (unsigned short) vertices.size();
		for ( int iLong = 0; iLong < longDiv; iLong++ )
		{
			vertices.emplace_back();
			auto vec = DirectX::XMVector4Transform( base, DirectX::XMMatrixRotationZ( longitudeAngle * iLong ) );
			DirectX::XMStoreFloat4( &vertices.back().position, vec );
		}

		std::vector<unsigned short> indices;

		// cone indices
		for ( unsigned short i = 0; i < longDiv * 3; i++ )
		{
			indices.push_back( i + iCone );
		}

		// base indices
		for ( unsigned short iLong = 0; iLong < longDiv; iLong++ )
		{
			indices.push_back( iBaseCenter );
			indices.push_back( ( iLong + 1 ) % longDiv + iBaseEdge );
			indices.push_back( iLong + iBaseEdge );
		}

		return { std::move( vertices ),std::move( indices ) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};