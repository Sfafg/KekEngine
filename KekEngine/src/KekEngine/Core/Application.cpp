#include "Application.h"
#include "KekEngine/Graphics/Context.h"
#include "GLFW/glfw3.h"

using namespace Kek;
int main()
{
	Application app;
	if(!WindowContext::Init(4, 6));
	Setup();
	GraphicsContext::Init();

	while(!app.Closed())
	{
		Update();
		glfwPollEvents();
	}
	app.Quit();
}

namespace Kek
{
	Window* window;
	int windowCount;

	void Application::Realloc(int size)
	{
		if(windowCount == size)return;
		windowCount = size;
		void* ptr = realloc(window, size * sizeof(Window));
		if(ptr != NULL)
		{
			window = (Window*)ptr;
		}
	}

	Application::Application()
	{
	}

	void Application::Quit()
	{
		delete window;
		WindowContext::Terminate();
	}

	bool Application::Closed()
	{
		for(int i = 0; i < windowCount; i++)
		{
			if(!window[i].Closed()) return false;
		}
		return true;
	}

	int Application::WindowCount() { return windowCount; }

	void Application::AddWindow(const Window& _window)
	{
		Realloc(windowCount + 1);
		window[windowCount - 1] = _window;
	}
	Window& Application::GetWindow(int index)
	{
		return window[index];
	}
	void Application::DeleteWindow(int index)
	{
		for(int i = index; i < windowCount - 1; i++)
		{
			window[i] = window[i + 1];
		}
		Realloc(windowCount - 1);
	}
}
