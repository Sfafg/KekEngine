#include "Application.h"
#include "KekEngine/Graphics/Context.h"
#include "GLFW/glfw3.h"
#pragma comment(linker, "/ENTRY:mainCRTStartup")
using namespace Kek;

Window** window = NULL;
int windowCount = 0;
bool shouldClose = false;

Event<>& Application::FrameEndEvent() {static Event<> event; return event;}

void Quit()
{
	for(int i = 0; i < windowCount; i++)
	{
		window[i]->Destroy();
		delete window[i];
	}
	SystemContext::Terminate();
}

int main()
{
	Setup();
	while(!Application::Closed())
	{
		glfwPollEvents();

		Update();
		Application::FrameEndEvent()();

		for(int i = Application::WindowCount() - 1; i >= 0; i--)
			if(Application::GetWindow(i).Closed())
				Application::DeleteWindow(i);
	}
	Quit();

	return 0;
}

namespace Kek
{
	Application::Application(const Window& window)
	{
		AddWindow(window);
	}
	Application::Application()
	{
		SystemContext::Init();
	}

	const Window& Application::operator[](unsigned int index) const
	{
		return *window[index];
	}
	Window& Application::operator[](unsigned int index)
	{
		return *window[index];
	}

	void Application::Close()
	{
		shouldClose = true;
	}
	bool Application::Closed()
	{
		return shouldClose;
	}

	int Application::WindowCount() { return windowCount; }

	void Application::AddWindow(Window* _window)
	{
		Realloc(windowCount + 1);
		window[windowCount - 1] = _window;
	}
	void Application::AddWindow(const Window& _window)
	{
		Realloc(windowCount + 1);
		window[windowCount - 1] = new Window(_window);
		GraphicsContext::Init(4,6);
	}
	Window& Application::GetWindow(int index)
	{
		return *window[index];
	}
	void Application::DeleteWindow(int index)
	{
		window[index]->Destroy();
		delete window[index];

		for(int i = index; i < windowCount - 1; i++)
		{
			window[i] = window[i + 1];
		}
		Realloc(windowCount - 1);
	}

	void Application::Realloc(int size)
	{
		if(windowCount == size)return;
		windowCount = size;
		void* ptr = realloc(window, size * sizeof(Window*));
		if(ptr != NULL)
		{
			window = (Window**)ptr;
		}
	}
}