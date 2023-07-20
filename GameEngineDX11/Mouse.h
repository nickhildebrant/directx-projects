#pragma once
#include <queue>

class Mouse {
	friend class Window;

public:
	class Event {
	public:
		enum class Type
		{
			LPress, LRelease, RPress, RRelease,
			MPress, MRelease, WheelUp, WheelDown,
			Move, Enter, Leave, Invalid
		};

		Event()
			: type( Type::Invalid ), x( 0 ), y( 0 ),
			isLeftPressed( false ), isRightPressed( false ), isMiddlePressed( false )
		{}

		Event( Type type, const Mouse& parent )
			: type( type ), x( parent.x ), y( parent.y ),
			isLeftPressed( parent.isLeftPressed ), isRightPressed( parent.isRightPressed ),
			isMiddlePressed( parent.isMiddlePressed )
		{}

	public:
		bool IsValid() { return type != Type::Invalid; }
		Type GetType() { return type; }

		int GetMouseX() { return x; }
		int GetMouseY() { return y; }
		std::pair<int, int> GetPos() { return { x, y }; }

		bool LeftIsPressed() { return isLeftPressed; }
		bool RightIsPressed() { return isRightPressed; }
		bool MiddleIsPressed() { return isMiddlePressed; }

	private:
		Type type;
		int x;
		int y;
		bool isLeftPressed;
		bool isRightPressed;
		bool isMiddlePressed;
	};

	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	int GetMouseX();
	int GetMouseY();
	int GetPrevX();
	int GetPrevY();
	std::pair<int, int> GetPos();
	std::pair<int, int> GetPrevPos();

	bool IsInWindow();

	bool LeftIsPressed();
	bool RightIsPressed();
	bool MiddleIsPressed();

	Mouse::Event Read();
	bool IsEmpty() { return buffer.empty(); }
	void Flush();

private:
	void OnMouseMove( int x, int y );

	void OnMouseLeave();
	void OnMouseEnter();

	void OnLeftPressed( int x, int y );
	void OnLeftReleased( int x, int y );

	void OnRightPressed( int x, int y );
	void OnRightReleased( int x, int y );

	void OnMiddlePressed( int x, int y );
	void OnMiddleReleased( int x, int y );

	void OnWheelUp( int x, int y );
	void OnWheelDown( int x, int y );

	void OnWheelDelta( int x, int y, int delta );

	void TrimBuffer();

	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> buffer;

	int x;
	int y;

	int prevX;
	int prevY;

	int wheelDeltaCry = 0;

	bool isInWindow = false;

	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isMiddlePressed = false;
};