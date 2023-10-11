#pragma once
#include <vector>
#include <array>
#include "IndexedTriangleList.h"
#include "VertexLayout.h"

class Plane {
public:
	static IndexedTriangleList MakeTesselatedTextured( VertexHandler::VertexLayout layout, int xDivisions, int yDivisions )
	{
		assert( xDivisions >= 1 );
		assert( yDivisions >= 1 );

		constexpr float width = 1.0f;
		constexpr float height = 1.0f;

		const int numberVerticesX = xDivisions + 1;
		const int numberVerticesY = yDivisions + 1;

		VertexHandler::VertexBuffer vertexBuffer { std::move( layout ) };
		{
			const float sideX = width / 2.0f;
			const float sideY = height / 2.0f;

			const float xDivisionSize = width / float( xDivisions );
			const float yDivisionSize = height / float( yDivisions );

			const float xDivisionTexCoord = 1.0f / float( xDivisions );
			const float yDivisionTexCoord = 1.0f / float( yDivisions );

			for ( int y = 0, i = 0; y < numberVerticesY; y++ )
			{
				float y_pos = float( y ) * yDivisionSize - sideY;
				float y_pos_tc = 1.0f - float( y ) * yDivisionTexCoord;
				for ( int x = 0; x < numberVerticesX; x++, i++ )
				{
					float x_pos = float( x ) * xDivisionSize - sideX;
					float x_pos_tc = float( x ) * xDivisionTexCoord;

					vertexBuffer.EmplaceBack( DirectX::XMFLOAT4{ x_pos, y_pos, 0.0f, 1.0f }, DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 0.0f }, DirectX::XMFLOAT2{ x_pos_tc, y_pos_tc } );
				}
			}
		}

		std::vector<unsigned short> indices;
		float divisions = xDivisions * yDivisions;
		indices.reserve( ( divisions * divisions ) * 6 );
		{
			const auto vertices2i = [numberVerticesX]( size_t x, size_t y )
			{
				return (unsigned short) ( y * numberVerticesX + x );
			};
			
			for ( size_t y = 0; y < yDivisions; y++ )
			{
				for ( size_t x = 0; x < xDivisions; x++ )
				{
					const std::array<unsigned short, 4> indexArray =
					{ 
						vertices2i( x,y ),
						vertices2i( x + 1,y ),
						vertices2i( x,y + 1 ),
						vertices2i( x + 1,y + 1 )
					};

					indices.push_back( indexArray[0] );
					indices.push_back( indexArray[2] );
					indices.push_back( indexArray[1] );
					indices.push_back( indexArray[1] );
					indices.push_back( indexArray[2] );
					indices.push_back( indexArray[3] );
				}
			}
		}

		return{ std::move( vertexBuffer ),std::move( indices ) };
	}

	static IndexedTriangleList Make()
	{
		VertexHandler::VertexLayout layout;
		layout.Append( VertexHandler::VertexLayout::VertexLayout::Position3D );
		layout.Append( VertexHandler::VertexLayout::VertexLayout::Normal );
		layout.Append( VertexHandler::VertexLayout::VertexLayout::Texture2D );

		return MakeTesselatedTextured( std::move( layout ), 1, 1 );
	}
};