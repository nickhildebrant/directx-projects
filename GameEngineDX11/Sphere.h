#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Sphere {
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated( int latitudeDivisions, int longitudeDivisions )
	{
		const float PI = 3.14159265f;

		assert( latitudeDivisions >= 3 );
		assert( longitudeDivisions >= 3 );

		constexpr float radius = 1.0f;
		const auto base = DirectX::XMVectorSet( 0.0f, 0.0f, radius, 0.0f );
		const float latitudeAngle = PI / latitudeDivisions;
		const float longitudeAngle = 2.0f * PI / longitudeDivisions;

		std::vector<V> vertices;
		for ( int iLatitude = 1; iLatitude < latitudeDivisions; iLatitude++ )
		{
			const auto latitudeBase = DirectX::XMVector3Transform( base, DirectX::XMMatrixRotationX( latitudeAngle * iLatitude ) );

			for ( int iLongitude = 0; iLongitude < longitudeDivisions; iLongitude++ )
			{
				vertices.emplace_back();
				auto vec = DirectX::XMVector3Transform( latitudeBase, DirectX::XMMatrixRotationZ( longitudeAngle * iLongitude ) );
				DirectX::XMStoreFloat3( &vertices.back().position, vec );
			}
		}

		// add the cap vertices
		const auto iNorthPole = (unsigned short) vertices.size();
		vertices.emplace_back();

		DirectX::XMStoreFloat3( &vertices.back().position, base );
		const auto iSouthPole = (unsigned short) vertices.size();
		vertices.emplace_back();

		DirectX::XMStoreFloat3( &vertices.back().position, DirectX::XMVectorNegate( base ) );

		const auto calcIdx = [latitudeDivisions, longitudeDivisions]( unsigned short iLatitude, unsigned short iLongitude ) { return iLatitude * longitudeDivisions + iLongitude; };

		std::vector<unsigned short> indices;
		for ( unsigned short iLatitude = 0; iLatitude < latitudeDivisions - 2; iLatitude++ )
		{
			for ( unsigned short iLongitude = 0; iLongitude < longitudeDivisions - 1; iLongitude++ )
			{
				indices.push_back( calcIdx( iLatitude, iLongitude ) );
				indices.push_back( calcIdx( iLatitude + 1, iLongitude ) );
				indices.push_back( calcIdx( iLatitude, iLongitude + 1 ) );
				indices.push_back( calcIdx( iLatitude, iLongitude + 1 ) );
				indices.push_back( calcIdx( iLatitude + 1, iLongitude ) );
				indices.push_back( calcIdx( iLatitude + 1, iLongitude + 1 ) );
			}

			// wrap band
			indices.push_back( calcIdx( iLatitude, longitudeDivisions - 1 ) );
			indices.push_back( calcIdx( iLatitude + 1, longitudeDivisions - 1 ) );
			indices.push_back( calcIdx( iLatitude, 0 ) );
			indices.push_back( calcIdx( iLatitude, 0 ) );
			indices.push_back( calcIdx( iLatitude + 1, longitudeDivisions - 1 ) );
			indices.push_back( calcIdx( iLatitude + 1, 0 ) );
		}

		// cap fans
		for ( unsigned short iLongitude = 0; iLongitude < longitudeDivisions - 1; iLongitude++ )
		{
			// north
			indices.push_back( iNorthPole );
			indices.push_back( calcIdx( 0, iLongitude ) );
			indices.push_back( calcIdx( 0, iLongitude + 1 ) );

			// south
			indices.push_back( calcIdx( latitudeDivisions - 2, iLongitude + 1 ) );
			indices.push_back( calcIdx( latitudeDivisions - 2, iLongitude ) );
			indices.push_back( iSouthPole );
		}

		// wrap triangles
		// north
		indices.push_back( iNorthPole );
		indices.push_back( calcIdx( 0, longitudeDivisions - 1 ) );
		indices.push_back( calcIdx( 0, 0 ) );

		// south
		indices.push_back( calcIdx( latitudeDivisions - 2, 0 ) );
		indices.push_back( calcIdx( latitudeDivisions - 2, longitudeDivisions - 1 ) );
		indices.push_back( iSouthPole );

		return { std::move( vertices ),std::move( indices ) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>( 12, 24 );
	}
};