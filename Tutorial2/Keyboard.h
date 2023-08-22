#pragma once
#include <optional>
#include <queue>
#include <bitset>

class Keyboard {
	friend class Window;

public:
	class Event {
	public:
		enum class Type { Press, Release, Invalid };

	private:
		Type type;
		unsigned char code;

	public:
		Event() : type(Type::Invalid), code(0u) {}
		Event(Type type, unsigned char code) : type(type), code(code) {}

		bool IsPressed() const { return type == Type::Press; }
		bool IsReleased() const { return type == Type::Release; }
		bool IsValid() const { return type != Type::Invalid; }

		unsigned char GetCode() const { return code; }
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// *** Event methods
	// key events
	bool isKeyPressed(unsigned char keycode);
	bool isKeyEmpty();
	std::optional<Event> ReadKey();
	void ClearKeyQueue();

	// char events
	bool isCharEmpty();
	std::optional<char> ReadChar();
	void ClearCharQueue();

	void ClearQueues();
	// ******

	// Autorepeat
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool GetAutorepeatStatus();

private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char character);
	void ClearState();	// Clears the bitset with key states

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);	// Trims queue back to 16

	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool isAutorepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};