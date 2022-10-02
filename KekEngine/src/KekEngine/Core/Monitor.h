#pragma once
#include "KekEngine/Maths/Vector.h"
struct GLFWmonitor;

namespace Kek
{
	class Monitor;
	namespace Monitors
	{
		Monitor* GetArray();
		Monitor* GetArray(int& size);
		Monitor Get(int index);
	};

	class Monitor
	{
		GLFWmonitor* glfwMonitor;

		public:
		Monitor();
		Monitor(int index);

		const char* Name() const;
		vec2i Size() const;
		vec2i PhisicalSize() const;
		vec2i Position() const;
		int RefreshRate() const;

		operator GLFWmonitor* ();
	};
}