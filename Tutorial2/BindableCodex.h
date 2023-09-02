#pragma once
#include <memory>
#include <unordered_map>
#include <type_traits>
#include "Bindable.h"

class Codex {
public:
	template<class T, typename...Params>
	static std::shared_ptr<T> Resolve( Renderer& renderer, Params&&...params )
	{
		static_assert( std::is_base_of<Bindable, T>::value, "Can only resolve Bindable child classes." );
		return Get().Resolve_<T>( renderer, std::forward<Params>( params )... );
	}

private:
	std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;

	template<class T, typename...Params>
	std::shared_ptr<T> Resolve_( Renderer& renderer, Params&&...p )
	{
		const auto key = T::GenerateUID( std::forward<Params>( p )... );
		const auto i = binds.find( key );
		if ( i == binds.end() )
		{
			auto bind = std::make_shared<T>( renderer, std::forward<Params>( p )... );
			binds[key] = bind;
			return bind;
		}
		else
		{
			return std::static_pointer_cast<T>( i->second );
		}
	}

	static Codex& Get()
	{
		static Codex codex;
		return codex;
	}
};