#pragma once
#include "Keys.h"

namespace Kek
{
	extern int StateToWindows(int state);
	extern int WindowsToState(int glfwState);

	extern int KeyToWindows(int key);
	extern int WindowsToKey(int glfwKey);
}