#pragma once
#include "Window.h"

namespace Kek
{
	void Setup();
	void Update();
	class Application
	{
		static void Realloc(int size);
		public:

		Application();
		static void Quit();

		static bool Closed();

		static int WindowCount();
		static void AddWindow(const Window& window);
		static Window& GetWindow(int index);
		static void DeleteWindow(int index);
	};
}
