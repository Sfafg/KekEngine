#pragma once
#include "Window.h"
#include <vector>

namespace Kek
{
	void Setup();
	void Update();
	class Application
	{
		static std::vector<Window *> windowArray;
		static bool shouldQuit;

		struct Init
		{
			Init();
		};

	public:
		static Event<> &OnFrameEnd();

		Application();
		Application(const Window &window, Init init = Init());

		const Window &operator[](unsigned int index) const;
		Window &operator[](unsigned int index);

		static void Quit();
		static bool IsQuitting();

		static unsigned int WindowCount();
		static void AddWindow(const Window &window);
		static void AddWindow(Window *_window);
		static Window &GetWindow(int index);
		static void DeleteWindow(int index);
		static void DeleteWindow(Window* _window);
	};
}