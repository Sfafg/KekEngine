#pragma once
#include "KekEngine/Core/Events.h"
#include "KekEngine/Core/Windows_Keys.h"
#include "KekEngine/Maths/Vector.h"

namespace Kek
{
	namespace PlatformIO
	{
		extern void Init();
		extern Event<char, char, int>& KeyEvent();
		extern Event<vec2i>& MouseMoveEvent();
		extern Event<vec2i>& MouseScrollEvent();

		extern void MapKey(int keyA, int keyB);
		extern void SetKey(KeyData key);

		extern void SetScrollDelta(vec2i scrollDelta);
		extern void SetMousePos(vec2i pos);
		extern void SetMouseDelta(vec2i delta);
	}
}