#include "Application.h"
#include "Context.h"
#include "GLFW/glfw3.h"
#pragma comment(linker, "/ENTRY:mainCRTStartup")
using namespace Kek;

int main()
{
	Setup();
	while (!Application::IsQuitting())
	{
		glfwPollEvents();

		Update();
		Application::FrameEndEvent()();

		for (int i = Application::WindowCount() - 1; i >= 0; i--)
			if (Application::GetWindow(i).Closed())
				Application::DeleteWindow(i);
	}
	for (int i = Application::WindowCount() - 1; i >= 0; i--)
	{
		Application::DeleteWindow(i);
	}
	SystemContext::Terminate();

	return 0;
}

namespace Kek
{
	Window **Application::windowArray = NULL;
	int Application::windowCount = 0;
	bool Application::shouldQuit = false;

	Application::Application(const Window &window)
	{
		SystemContext::Initialize();
		AddWindow(window);
	}
	Application::Application()
	{
		SystemContext::Initialize();
	}

	Event<> &Application::FrameEndEvent()
	{
		static Event<> event;
		return event;
	}

	const Window &Application::operator[](unsigned int index) const
	{
		return *windowArray[index];
	}
	Window &Application::operator[](unsigned int index)
	{
		return *windowArray[index];
	}

	void Application::Quit()
	{
		shouldQuit = true;
	}
	bool Application::IsQuitting()
	{
		return shouldQuit;
	}

	int Application::WindowCount() { return windowCount; }

	void Application::AddWindow(Window *_window)
	{
		Realloc(windowCount + 1);
		windowArray[windowCount - 1] = _window;
	}
	void Application::AddWindow(const Window &_window)
	{
		Realloc(windowCount + 1);
		windowArray[windowCount - 1] = new Window(_window);
	}
	Window &Application::GetWindow(int index)
	{
		return *windowArray[index];
	}
	void Application::DeleteWindow(int index)
	{
		windowArray[index]->Destroy();
		delete windowArray[index];

		for (int i = index; i < windowCount - 1; i++)
		{
			windowArray[i] = windowArray[i + 1];
		}
		Realloc(windowCount - 1);
	}

	void Application::Realloc(int size)
	{
		if (windowCount == size)
			return;
		windowCount = size;
		void *ptr = realloc(windowArray, size * sizeof(Window *));
		if (ptr != NULL)
		{
			windowArray = (Window **)ptr;
		}
	}
}