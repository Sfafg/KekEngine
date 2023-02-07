#pragma once
#include "KekEngine/Maths/Vector.h"
#include "Events.h"
#include "Monitor.h"
#include "Flag.h"
#include "Keys.h"

namespace Kek
{
	enum WindowStyle
	{
		None = 0,
		Fullscreen = 1,
		Borderless = 2,
		Floating = 4,
		Transparent = 8,
		Nonresizable = 16,
		Windowed_Fullscreen = Fullscreen | Borderless,
		Floating_Borderless = Floating | Borderless
	};
	class Window
	{
		void *window;
		const char *name;

	public:
		Window();
		Window(const char *title, vec2i size, FlagSet style = None, Monitor monitor = Monitor());
		Window(const char *title, vec2f size, FlagSet style = None, Monitor monitor = Monitor());
		Window(const char *title, FlagSet style = Fullscreen, Monitor monitor = Monitor());

		Window(const Window &window);
		void operator=(const Window &window);

		operator void *();

		const char *Title() const;
		void SetTitle(const char *title);

		vec2i Position() const;
		void SetPosition(vec2i pos);
		vec2i Size() const;
		void SetSize(vec2i size);
		float Opacity();
		void SetOpacity(float opacity);

		vec2f MousePosition() const;
		void SetMousePosition(vec2f mpos);

		bool GetKey(int key) const;
		bool GetButton(int button);

		void Focus();
		Monitor GetMonitor();
		void SetMonitor(Monitor monitor);

		void Clear(vec4f col);
		void SwapBuffers();
		void ContextCurrent();
		void Close();
		bool Closed() const;
		void Destroy();

		Event<vec2i, Window *> OnMouseMove;
		Event<vec2i, Window *> OnMove;
		Event<bool, Window *> OnMouseEnter;
		Event<char, int, char, FlagSet, Window *> OnKey;

		Event<vec2i, Window *> OnResize;
		Event<Window *> OnClose;
		Event<bool, Window *> OnMaximize;
		Event<bool, Window *> OnFocus;
	};
}