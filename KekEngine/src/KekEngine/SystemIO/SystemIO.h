#pragma once
#include "KekEngine/Core/Events.h"
#include "KekEngine/Core/Keys.h"
#include "KekEngine/Maths/Vector.h"
#include "KekEngine/Core/Flag.h"

namespace Kek
{
	namespace SystemIO
	{
		extern void Init();
		extern void PollEvents();

		extern Event<int, int, int, FlagSet> keyEvent;
		extern Event<vec2i, FlagSet> scrollWheelEvent;
		extern Event<vec2i, vec2i, FlagSet> mouseMoveEvent;

		extern FlagSet modulators;
		extern vec2i mousePos;
		extern vec2i mouseDelta;
		extern vec2i scrollDelta;
		extern int Key(int index);

		extern Event<int, int, int, FlagSet> keyEventAsync;
		extern Event<vec2i, FlagSet> scrollWheelEventAsync;
		extern Event<vec2i, vec2i, FlagSet> mouseMoveEventAsync;

		extern void SetMousePos(vec2i pos);
		extern void SetMouseDelta(vec2i delta);
		extern void SetScrollDelta(vec2i scrollDelta);
		extern void SetKey(KeyData key);
		extern void MapKey(int keyA, int keyB);
	}
}