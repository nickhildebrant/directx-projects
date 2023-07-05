#pragma once
#include <assert.h>
#include <memory>

class Surface {
public:
	class Color {
	public:
		unsigned int dword;

		constexpr Color() noexcept : dword() {}
		
		constexpr Color(const Color& color) noexcept : dword(color.dword) {}

		constexpr Color(unsigned int dw) noexcept : dword(dw) {}
	};
};