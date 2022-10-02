#pragma once
#include "KekEngine/Maths/Vector.h"
#include "KekEngine/Core/Events.h"
#include "KekEngine/Core/Monitor.h"
#include "KekEngine/Core/Flag.h"

struct GLFWwindow;

namespace Kek
{
	struct WindowStyle
	{
		const static char None = 0,
			Fullscreen = 1,
			Borderless = 2,
			Floating = 4,
			WindowedFullscreen = Fullscreen | Borderless,
			FloatingBorderless = Floating | Borderless;
	};

	class Window
	{
		GLFWwindow* glfwWindow;
		public:
		Window(const char* title, FlagSet style, Monitor monitor = Monitor());
		Window(const char* title, FlagSet style, vec2i size, vec2i pos = { 0,0 }, Monitor monitor = Monitor());
		Window(const char* title, FlagSet style, vec2f size, vec2f pos = { 0,0 }, Monitor monitor = Monitor());
		Window(const char* title, vec2i size, vec2i pos = { 0,0 }, Monitor monitor = Monitor());
		Window(const char* title, vec2f size, vec2f pos = { 0,0 }, Monitor monitor = Monitor());

		Event<vec2i, Window*> OnMouseMove;
		Event<vec2i, Window*> OnMove;
		Event<bool, Window*> OnMouseEnter;

		Event<vec2i, Window*> OnResize;
		Event<Window*> OnClose;
		Event<bool, Window*> OnMaximize;
		Event<bool, Window*> OnFocus;

		operator GLFWwindow* ();

		vec2i Position();
		void SetPosition(vec2i pos);
		vec2i Size();
		void SetSize(vec2i size);

		vec2f MousePosition();
		void SetMousePosition(vec2f mpos);

		bool GetKey(int key);
		bool GetButton(int button);

		void Focus();
		Monitor GetMonitor();
		void SetMonitor(Monitor monitor);

		void SetAttribute(int attrib, int value);
		void ContextCurrent();
		void Close();
		bool Closed();
		void Destroy();
	};
}