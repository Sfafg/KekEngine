#include "SystemIO.h"
#include <map>
#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Application.h"
#include "PlatformIO.h"

Kek::vec2i scrollDelta;
Kek::vec2i mousePos;
Kek::vec2i lastMousePos;
char keyState[128] = { 0 };

void ScrollCallback(Kek::vec2i delta)
{
	scrollDelta = delta;
}
void MouseMoveCallback(Kek::vec2i pos)
{
	mousePos = pos;
}
void KeyCallback(char key, char state, int code)
{
	if(state == Kek::State_Press && keyState[key] == Kek::State_Release)
	{
		keyState[key] = Kek::State_Click;
	}
	else
	{
		keyState[key] = state;
	}
}

static void FrameEnd()
{
	lastMousePos = mousePos;
	scrollDelta = Kek::vec2i(0, 0);
	for(int i = 0; i < 128; i++)
	{
		if(keyState[i] == Kek::State_Click) keyState[i] = Kek::State_Press;
	}
}
namespace Kek
{
	namespace SystemIO
	{
		bool initialized = false;
		void Init()
		{
#ifndef NDEBUG
			if(initialized)
			{
				Log<Error>("You must NOT initialize SystemIO more then once.");
				return;
			}
			initialized = true;
#endif // !NDEBUG
			PlatformIO::Init();

			MouseScrollEvent() += ScrollCallback;
			MouseMoveEvent() += MouseMoveCallback;
			KeyEvent() += KeyCallback;

			Application::FrameEndEvent() += FrameEnd;
		}

		Event<vec2i>& MouseMoveEvent()
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
			}
#endif // !NDEBUG
			return PlatformIO::MouseMoveEvent();
		}
		Event<vec2i>& MouseScrollEvent()
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
			}
#endif // !NDEBUG
			return PlatformIO::MouseScrollEvent();
		}
		Event<char, char, int>& KeyEvent()
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
			}
#endif // !NDEBUG
			return PlatformIO::KeyEvent();
		}

		vec2i MousePos()
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
			}
#endif // !NDEBUG
			return mousePos;
		}
		vec2i MouseDelta()
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
			}
#endif // !NDEBUG
			return mousePos - lastMousePos;
		}
		vec2i ScrollDelta()
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
			}
#endif // !NDEBUG
			return scrollDelta;
		}
		char Key(char index)
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
			}
#endif // !NDEBUG
			return keyState[index];
		}

		void SetMousePos(vec2i pos)
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
				return;
			}
#endif // !NDEBUG
			PlatformIO::SetMousePos(pos);
		}
		void SetMouseDelta(vec2i delta)
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
				return;
			}
#endif // !NDEBUG
			PlatformIO::SetMouseDelta(delta);
		}
		void SetScrollDelta(vec2i scrollDelta)
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
				return;
			}
#endif // !NDEBUG
			PlatformIO::SetScrollDelta(scrollDelta);
		}
		void SetKey(KeyData key)
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
				return;
			}
#endif // !NDEBUG
			if(key.index == Key_None) return;
			PlatformIO::SetKey(key);
		}
		void MapKey(int keyA, int keyB)
		{
#ifndef NDEBUG
			if(!initialized)
			{
				Log<Error>("SystemIO is uninitialized.");
				return;
			}
#endif // !NDEBUG
			PlatformIO::MapKey(keyA, keyB);
		}
	}
}