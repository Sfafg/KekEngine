#pragma once
#include "KekEngine/Core/Events.h"
#include "KekEngine/Core/Keys.h"
#include "KekEngine/Maths/Vector.h"
namespace Kek
{
	namespace SystemIO
	{
		extern void Init();
		extern Event<vec2i>& MouseMoveEvent();
		extern Event<vec2i>& MouseScrollEvent();
		extern Event<char, char, int>& KeyEvent();

		extern vec2i MousePos();
		extern vec2i MouseDelta();
		extern vec2i ScrollDelta();
		extern char Key(char index);

		extern void SetMousePos(vec2i pos);
		extern void SetMouseDelta(vec2i delta);
		extern void SetScrollDelta(vec2i scrollDelta);
		extern void SetKey(KeyData key);
		extern void MapKey(int keyA, int keyB);
		
	}
}

