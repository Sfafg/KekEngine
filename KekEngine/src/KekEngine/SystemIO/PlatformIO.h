#pragma once
#include "KekEngine/Core/Events.h"
#include "KekEngine/Core/Windows_Keys.h"
#include "KekEngine/Maths/Vector.h"

namespace Kek
{
	namespace PlatformIO
	{
		extern void Init();
		extern bool KeyCallback(int key, int state, int code, bool isSimulated);
		extern bool MouseMoveCallback(vec2i position, bool isSimulated);
		extern bool ScrollCallback(vec2i delta, bool isSimulated);

		extern void SetKey(KeyData key);
		extern void SetScrollDelta(vec2i scrollDelta);
		extern void SetMousePos(vec2i pos);
		extern void SetMouseDelta(vec2i delta);
	}
}