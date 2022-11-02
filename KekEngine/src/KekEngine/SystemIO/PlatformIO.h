#pragma once
#include "KekEngine/Core/Events.h"
#include "KekEngine/Core/Windows_Keys.h"
#include "KekEngine/Maths/Vector.h"

namespace Kek
{
	namespace PlatformIO
	{
		extern void Init();
		extern FunctionPointer<bool, int, int, int, bool> keyCallback;
		extern FunctionPointer<bool, vec2i, bool> mouseMoveCallback;
		extern FunctionPointer<bool, vec2i, bool> scrollWheelCallback;

		extern void SetKey(KeyData key);
		extern void SetScrollDelta(vec2i scrollDelta);
		extern void SetMousePos(vec2i pos);
		extern void SetMouseDelta(vec2i delta);
	}
}