#pragma once
#include "Window.h"
#include "Events.h"

namespace Kek
{
	void Setup();
	void Update();
	class Application
	{
		static Window **windowArray;
		static int windowCount;
		static bool shouldQuit;

	public:
		static Event<> &FrameEndEvent();

		Application(const Window &window);
		Application();

		const Window &operator[](unsigned int index) const;
		Window &operator[](unsigned int index);

		static void Quit();
		static bool IsQuitting();

		static int WindowCount();
		static void AddWindow(const Window &window);
		static void AddWindow(Window *_window);
		static Window &GetWindow(int index);
		static void DeleteWindow(int index);

	private:
		static void Realloc(int size);
	};
}