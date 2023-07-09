#include "Keyboard.h"

bool Keyboard::isKeyPressed( unsigned char keycode ) { return keyStates[keycode]; }

bool Keyboard::isKeyEmpty() { return keyBuffer.empty(); }

bool Keyboard::isCharEmpty() { return charBuffer.empty(); }

// Clear queues
void Keyboard::ClearKeyQueue() { keyBuffer = std::queue<Event>(); }
void Keyboard::ClearCharQueue() { charBuffer = std::queue<char>(); }
void Keyboard::ClearQueues()
{
	Keyboard::ClearKeyQueue();
	Keyboard::ClearCharQueue();
}

// Auto repeat methods
void Keyboard::EnableAutorepeat() { isAutorepeatEnabled = true; }
void Keyboard::DisableAutorepeat() { isAutorepeatEnabled = false; }
bool Keyboard::GetAutorepeatStatus() { return isAutorepeatEnabled; }

Keyboard::Event Keyboard::ReadKey()
{
	if ( keyBuffer.size() > 0u )
	{
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop(); // remove key from queue
		return e;
	}
	else return Keyboard::Event();
}

char Keyboard::ReadChar()
{
	if ( charBuffer.size() > 0u )
	{
		unsigned char charcode = charBuffer.front();
		charBuffer.pop(); // remove char from queue
		return charcode;
	}
	else return 0;
}

void Keyboard::OnKeyPressed( unsigned char keycode )
{
	keyStates[keycode] = true;
	keyBuffer.push( Keyboard::Event( Keyboard::Event::Type::Press, keycode ) );
	TrimBuffer( keyBuffer );
}

void Keyboard::OnKeyReleased( unsigned char keycode )
{
	keyStates[keycode] = false;
	keyBuffer.push( Keyboard::Event( Keyboard::Event::Type::Release, keycode ) );
	TrimBuffer( keyBuffer );
}

void Keyboard::OnChar( char character )
{
	charBuffer.push( character );
	TrimBuffer( charBuffer );
}

void Keyboard::ClearState() { keyStates.reset(); }

template<typename T>
void Keyboard::TrimBuffer( std::queue<T>& buffer )
{
	while ( buffer.size() > bufferSize )
	{
		buffer.pop();
	}
}