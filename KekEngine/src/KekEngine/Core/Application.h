#pragma once
#include "Window.h"
#include "Events.h"

namespace Kek
{
	void Setup();
	void Update();
	class Application
	{
		public:
		static Event<>& FrameEndEvent();

		Application(const Window& window);
		Application();

		const Window& operator[](unsigned int index) const;
		Window& operator[](unsigned int index);

		static void Close();
		static bool Closed();

		static int WindowCount();
		static void AddWindow(const Window& window);
		static void AddWindow(Window* _window);
		static Window& GetWindow(int index);
		static void DeleteWindow(int index);

		private:
		static void Realloc(int size);
	};
}