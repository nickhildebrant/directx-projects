#include <Windows.h>
#include "Mouse.h"

int Mouse::GetMouseX() { return x; }
int Mouse::GetMouseY() { return y; }
std::pair<int, int> Mouse::GetPos() { return {x, y}; }

std::optional<Mouse::RawDelta> Mouse::ReadRawDelta()
{
	if ( rawDeltaBuffer.empty() )
	{
		return std::nullopt;
	}

	const RawDelta delta = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return delta;
}

bool Mouse::IsInWindow() { return isInWindow; }

bool Mouse::LeftIsPressed() { return isLeftPressed; }
bool Mouse::RightIsPressed() { return isRightPressed; }
bool Mouse::MiddleIsPressed() { return isMiddlePressed; }

Mouse::Event Mouse::Read()
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else return Mouse::Event();
}

void Mouse::Flush() { buffer = std::queue<Event>(); }

void Mouse::TrimBuffer()
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void Mouse::TrimRawInputBuffer()
{
	while ( rawDeltaBuffer.size() > bufferSize )
	{
		rawDeltaBuffer.pop();
	}
}

void Mouse::OnMouseMove(int mouseX, int mouseY)
{
	x = mouseX;
	y = mouseY;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter()
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave()
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int mouseX, int mouseY)
{
	isLeftPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int mouseX, int mouseY)
{
	isLeftPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int mouseX, int mouseY)
{
	isRightPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int mouseX, int mouseY)
{
	isRightPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int mouseX, int mouseY)
{
	isMiddlePressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int mouseX, int mouseY)
{
	isMiddlePressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta)
{
	wheelDeltaCry += delta;

	while (wheelDeltaCry >= WHEEL_DELTA)
	{
		wheelDeltaCry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}

	while (wheelDeltaCry <= -WHEEL_DELTA)
	{
		wheelDeltaCry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::OnRawDelta( int dx, int dy )
{
	rawDeltaBuffer.push( { dx,dy } );
	TrimBuffer();
}