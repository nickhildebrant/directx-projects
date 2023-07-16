#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Cone {
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated( int longitudeDivisions )
	{
		assert( longitudeDivisions >= 3 );

		const float PI = 3.14159265f;
		const auto base = DirectX::XMVectorSet( 1.0f, 0.0f, -1.0f, 0.0f );
		const float longitudeAngle = 2.0f * PI / longitudeDivisions;

		// base vertices
		std::vector<V> vertices;
		for ( int iLong = 0; iLong < longitudeDivisions; iLong++ )
		{
			vertices.emplace_back();
			auto vec = DirectX::XMVector3Transform( base, DirectX::XMMatrixRotationZ( longitudeAngle * iLong ) );
			DirectX::XMStoreFloat3( &vertices.back().position, vec );
		}

		// the center
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,-1.0f };
		const auto iCenter = (unsigned short) ( vertices.size() - 1 );

		// the tip :darkness:
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,1.0f };
		const auto iTip = (unsigned short) ( vertices.size() - 1 );


		// base indices
		std::vector<unsigned short> indices;
		for ( unsigned short iLong = 0; iLong < longitudeDivisions; iLong++ )
		{
			indices.push_back( iCenter );
			indices.push_back( ( iLong + 1 ) % longitudeDivisions );
			indices.push_back( iLong );
		}

		// cone indices
		for ( unsigned short iLong = 0; iLong < longitudeDivisions; iLong++ )
		{
			indices.push_back( iLong );
			indices.push_back( ( iLong + 1 ) % longitudeDivisions );
			indices.push_back( iTip );
		}

		return { std::move( vertices ), std::move( indices ) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>( 24 );
	}
};