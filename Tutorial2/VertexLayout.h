#pragma once
#include <vector>
#include <DirectXMath.h>
#include <type_traits>

namespace VertexHandler
{
	struct RGBAColor {
		float r;
		float g;
		float b;
		float a;
	};

	class VertexLayout {
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float4Color,
			RGBAColor,
		};

		class Element {
		public:
			Element( ElementType type, size_t offset ) : type( type ), offset( offset ) {}

			size_t GetOffsetAfter() const noexcept
			{
				return offset + Size();
			}

			size_t GetOffset() const
			{
				return offset;
			}

			size_t Size() const noexcept
			{
				return SizeOf( type );
			}

			static constexpr size_t SizeOf( ElementType type ) noexcept
			{
				switch ( type )
				{
				case Position2D:
					return sizeof( DirectX::XMFLOAT2 );

				case Position3D:
					return sizeof( DirectX::XMFLOAT4 );

				case Texture2D:
					return sizeof( DirectX::XMFLOAT2 );

				case Normal:
					return sizeof( DirectX::XMFLOAT4 );

				case Float4Color:
					return sizeof( DirectX::XMFLOAT4 );

				case RGBAColor:
					return sizeof( RGBAColor );
				}
				assert( "Invalid element type" && false );
				return 0u;
			}

			ElementType GetType() const noexcept
			{
				return type;
			}

		private:
			ElementType type;
			size_t offset;
		};

	public:
		template<ElementType Type>
		const Element& Resolve() const noexcept
		{
			for ( auto& e : elements )
			{
				if ( e.GetType() == Type )
				{
					return e;
				}
			}

			assert( "Could not resolve element type" && false );
			return elements.front();
		}

		const Element& ResolveByIndex( size_t i ) const noexcept
		{
			return elements[i];
		}

		template<ElementType Type>
		VertexLayout& Append() noexcept
		{
			elements.emplace_back( Type, Size() );
			return *this;
		}

		size_t Size() const noexcept
		{
			return elements.empty() ? 0u : elements.back().GetOffsetAfter();
		}

		size_t GetElementCount() const noexcept
		{
			return elements.size();
		}

	private:
		std::vector<Element> elements;
	};

	class Vertex {
		friend class VertexBuffer;
	public:
		template<VertexLayout::ElementType Type>
		auto& Attr() noexcept
		{
			const auto& element = layout.Resolve<Type>();
			auto pAttribute = pData + element.GetOffset();
			if constexpr ( Type == VertexLayout::Position2D )
			{
				return *reinterpret_cast<DirectX::XMFLOAT2*>( pAttribute );
			}
			else if constexpr ( Type == VertexLayout::Position3D )
			{
				return *reinterpret_cast<DirectX::XMFLOAT4*>( pAttribute );
			}
			else if constexpr ( Type == VertexLayout::Texture2D )
			{
				return *reinterpret_cast<DirectX::XMFLOAT2*>( pAttribute );
			}
			else if constexpr ( Type == VertexLayout::Normal )
			{
				return *reinterpret_cast<DirectX::XMFLOAT4*>( pAttribute );
			}
			else if constexpr ( Type == VertexLayout::Float4Color )
			{
				return *reinterpret_cast<DirectX::XMFLOAT4*>( pAttribute );
			}
			else if constexpr ( Type == VertexLayout::Float4Color )
			{
				return *reinterpret_cast<DirectX::XMFLOAT4*>( pAttribute );
			}
			else if constexpr ( Type == VertexLayout::RGBAColor )
			{
				return *reinterpret_cast<RGBAColor*>( pAttribute );
			}
			else
			{
				assert( "Bad element type" && false );
				return *reinterpret_cast<char*>( pAttribute );
			}
		}

		template<typename T>
		void SetAttributeByIndex( size_t i, T&& val ) noexcept
		{
			const auto& element = layout.ResolveByIndex( i );
			auto pAttribute = pData + element.GetOffset();

			switch ( element.GetType() )
			{
			case VertexLayout::Position2D:
				SetAttribute<DirectX::XMFLOAT2>( pAttribute, std::forward<T>( val ) );
				break;

			case VertexLayout::Position3D:
				SetAttribute<DirectX::XMFLOAT4>( pAttribute, std::forward<T>( val ) );
				break;

			case VertexLayout::Texture2D:
				SetAttribute<DirectX::XMFLOAT2>( pAttribute, std::forward<T>( val ) );
				break;

			case VertexLayout::Normal:
				SetAttribute<DirectX::XMFLOAT4>( pAttribute, std::forward<T>( val ) );
				break;

			case VertexLayout::Float4Color:
				SetAttribute<DirectX::XMFLOAT4>( pAttribute, std::forward<T>( val ) );
				break;

			case VertexLayout::RGBAColor:
				SetAttribute<RGBAColor>( pAttribute, std::forward<T>( val ) );
				break;

			default:
				assert( "Bad element type" && false );
			}
		}

	protected:
		Vertex( char* pData, const VertexLayout& layout ) noexcept : pData( pData ), layout( layout )
		{
			assert( pData != nullptr );
		}

	private:
		template<typename First, typename ...Rest>
		// enables parameter pack setting of multiple parameters by element index
		void SetAttributeByIndex( size_t i, First&& first, Rest&&... rest ) noexcept
		{
			SetAttributeByIndex( i, std::forward<First>( first ) );
			SetAttributeByIndex( i + 1, std::forward<Rest>( rest )... );
		}

		// helper to reduce code duplication in SetAttributeByIndex
		template<typename Dest, typename Src>
		void SetAttribute( char* pAttribute, Src&& val ) noexcept
		{
			if constexpr ( std::is_assignable<Dest, Src>::value )
			{
				*reinterpret_cast<Dest*>( pAttribute ) = val;
			}
			else
			{
				assert( "Parameter attribute type mismatch" && false );
			}
		}

		char* pData = nullptr;
		const VertexLayout& layout;
	};

	class ConstantVertex {
	public:
		ConstantVertex( const Vertex& vertex ) noexcept : vertex( vertex ) {}

		template<VertexLayout::ElementType Type>
		const auto& Attr() const noexcept
		{
			return const_cast<Vertex&>( vertex ).Attr<Type>();
		}

	private:
		Vertex vertex;
	};

	class VertexBuffer {
	public:
		VertexBuffer( VertexLayout layout ) noexcept : layout( std::move( layout ) ) {}

		const VertexLayout& GetLayout() const noexcept
		{
			return layout;
		}

		const char* GetData() const noexcept
		{
			return buffer.data();
		}

		size_t Size() const noexcept
		{
			return buffer.size() / layout.Size();
		}

		size_t SizeBytes() const noexcept
		{
			return buffer.size();
		}

		template<typename ...Params>
		void EmplaceBack( Params&&... params ) noexcept
		{
			assert( sizeof...( params ) == layout.GetElementCount() && "Parameter count doesn't match number of vertex elements" );
			buffer.resize( buffer.size() + layout.Size() );
			Back().SetAttributeByIndex( 0u, std::forward<Params>( params )... );
		}

		Vertex Back() noexcept
		{
			assert( buffer.size() != 0u );
			return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
		}

		Vertex Front() noexcept
		{
			assert( buffer.size() != 0u );
			return Vertex{ buffer.data(),layout };
		}

		Vertex operator[]( size_t i ) noexcept
		{
			assert( i < Size() );
			return Vertex{ buffer.data() + layout.Size() * i,layout };
		}

		ConstantVertex Back() const noexcept
		{
			return const_cast<VertexBuffer*>( this )->Back();
		}

		ConstantVertex Front() const noexcept
		{
			return const_cast<VertexBuffer*>( this )->Front();
		}

		ConstantVertex operator[]( size_t i ) const noexcept
		{
			return const_cast<VertexBuffer&>( *this )[i];
		}

	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
}