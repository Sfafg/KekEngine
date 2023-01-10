#pragma once
#include "KekEngine/Maths/Vector.h"
#include "KekEngine/Core/Events.h"
#include "KekEngine/Core/Monitor.h"
#include "KekEngine/Core/Flag.h"
#include "Keys.h"
#include "GLFW_Keys.h"

namespace Kek
{
	struct WindowStyle
	{
		const static char
			None = 0,
			Fullscreen = 1,
			Borderless = 2,
			Floating = 4,
			Transparent = 8,
			WindowedFullscreen = Fullscreen | Borderless,
			FloatingBorderless = Floating | Borderless;
	};

	class Window
	{
		void* window;
		const char* name;
		public:

		Window(const char* title, FlagSet style, Monitor monitor = Monitor());
		Window(const char* title, FlagSet style, vec2i size, vec2i pos = { -1,-1 }, Monitor monitor = Monitor());
		Window(const char* title, FlagSet style, vec2f size, vec2f pos = { -1,-1 }, Monitor monitor = Monitor());
		Window(const char* title, vec2i size, vec2i pos = { -1,-1 }, Monitor monitor = Monitor());
		Window(const char* title, vec2f size, vec2f pos = { -1,-1 }, Monitor monitor = Monitor());

		Window(const Window& window);
		void operator=(const Window& window);

		operator void* ();

		Event<vec2i, Window*> OnMouseMove;
		Event<vec2i, Window*> OnMove;
		Event<bool, Window*> OnMouseEnter;
		Event<char, int, char, FlagSet, Window*> OnKey;

		Event<vec2i, Window*> OnResize;
		Event<Window*> OnClose;
		Event<bool, Window*> OnMaximize;
		Event<bool, Window*> OnFocus;

		const char* Title() const;
		void SetTitle(const char* title);

		vec2i Position() const;
		void SetPosition(vec2i pos);
		vec2i Size() const;
		void SetSize(vec2i size);

		vec2f MousePosition() const;
		void SetMousePosition(vec2f mpos);

		bool GetKey(int key) const;
		bool GetButton(int button);

		void Focus();
		Monitor GetMonitor();
		void SetMonitor(Monitor monitor);

		void Clear(vec4f col);
		void SwapBuffers();
		void SetAttribute(int attrib, int value);
		void ContextCurrent();
		void Close();
		bool Closed() const;
		void Destroy();
	};
}