#pragma once
#include <DirectXMath.h>
#include <array>

#include "IndexedTriangleList.h"
#include "VertexLayout.h"

class Cube {
public:
	static IndexedTriangleList MakeIndependent( VertexHandler::VertexLayout layout )
	{
		using Type = VertexHandler::VertexLayout::ElementType;

		constexpr float side = 1.0f / 2.0f;

		VertexHandler::VertexBuffer vertices( std::move( layout ), 24u);
		vertices[0].Attr<Type::Position3D>() = {-side,-side,-side,1.0f};// 0 near side
		vertices[1].Attr<Type::Position3D>() = { side,-side,-side,1.0f };// 1
		vertices[2].Attr<Type::Position3D>() = { -side,side,-side,1.0f };// 2
		vertices[3].Attr<Type::Position3D>() = { side,side,-side,1.0f };// 3

		vertices[4].Attr<Type::Position3D>() = { -side,-side,side,1.0f };// 4 far side
		vertices[5].Attr<Type::Position3D>() = { side,-side,side,1.0f };// 5
		vertices[6].Attr<Type::Position3D>() = { -side,side,side,1.0f };// 6
		vertices[7].Attr<Type::Position3D>() = { side,side,side,1.0f };// 7

		vertices[8].Attr<Type::Position3D>() = { -side,-side,-side,1.0f };// 8 left side
		vertices[9].Attr<Type::Position3D>() = { -side,side,-side,1.0f };// 9
		vertices[10].Attr<Type::Position3D>() = { -side,-side,side,1.0f };// 10
		vertices[11].Attr<Type::Position3D>() = { -side,side,side,1.0f };// 11

		vertices[12].Attr<Type::Position3D>() = { side,-side,-side,1.0f };// 12 right side
		vertices[13].Attr<Type::Position3D>() = { side,side,-side,1.0f };// 13
		vertices[14].Attr<Type::Position3D>() = { side,-side,side,1.0f };// 14
		vertices[15].Attr<Type::Position3D>() = { side,side,side,1.0f };// 15

		vertices[16].Attr<Type::Position3D>() = { -side,-side,-side,1.0f };// 16 bottom side
		vertices[17].Attr<Type::Position3D>() = { side,-side,-side,1.0f };// 17
		vertices[18].Attr<Type::Position3D>() = { -side,-side,side,1.0f };// 18
		vertices[19].Attr<Type::Position3D>() = { side,-side,side,1.0f };// 19

		vertices[20].Attr<Type::Position3D>() = { -side,side,-side,1.0f };// 20 top side
		vertices[21].Attr<Type::Position3D>() = { side,side,-side,1.0f };// 21
		vertices[22].Attr<Type::Position3D>() = { -side,side,side,1.0f };// 22
		vertices[23].Attr<Type::Position3D>() = { side,side,side,1.0f };// 23

		return
		{
			std::move( vertices ),
			{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}

	static IndexedTriangleList MakeIndependentTextured()
	{
		using Type = VertexHandler::VertexLayout::ElementType;

		IndexedTriangleList itl = MakeIndependent( std::move( VertexHandler::VertexLayout{}
			.Append( Type::Position3D )
			.Append( Type::Normal )
			.Append( Type::Texture2D )
		) );

		itl.vertices[0].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[1].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[2].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[3].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		itl.vertices[4].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[5].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[6].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[7].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		itl.vertices[8].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[9].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[10].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[11].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		itl.vertices[12].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[13].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[14].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[15].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		itl.vertices[16].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[17].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[18].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[19].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		itl.vertices[20].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[21].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[22].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[23].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		return itl;
	}
};