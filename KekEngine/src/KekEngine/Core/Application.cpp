#include "Application.h"
#include "Context.h"
#include "GLFW/glfw3.h"
using namespace Kek;

int main()
{
	Setup();

	while (!Application::IsQuitting())
	{
		glfwPollEvents();

		Update();

		Application::OnFrameEnd()();

		// Delete marked windows.
		for (int i = Application::WindowCount() - 1; i >= 0; i--)
		{
			if (Application::GetWindow(i).Closed())
			{
				Application::GetWindow(i).Destroy();
				Application::DeleteWindow(i);
			}
		}
	}

	return 0;
}

namespace Kek
{
	std::vector<Window*> Application::windowArray;
	bool Application::shouldQuit = false;
	Application::Init::Init()
	{
		SystemContext::Initialize();
	}
	Application::Application()
	{
		SystemContext::Initialize();
	}
	Application::Application(const Window &window, Init init)
	{
		AddWindow(window);
		GraphicsContext::Initialize();
	}
	

	Event<> &Application::OnFrameEnd()
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

	unsigned int Application::WindowCount() { return windowArray.size(); }

	void OnWindowCloseListener(Window* window) { Application::DeleteWindow(window); window->Destroy(); }
	void Application::AddWindow(Window *_window)
	{
		_window->OnClose += OnWindowCloseListener;
		windowArray.push_back(_window);
	}
	void Application::AddWindow(const Window &_window)
	{
		AddWindow(new Window(_window));
	}
	Window &Application::GetWindow(int index)
	{
		return *windowArray[index];
	}
	void Application::DeleteWindow(int index)
	{
		windowArray.erase(windowArray.begin() + index);

		if (WindowCount() == 0)
			Quit();
	}
	void Application::DeleteWindow(Window* _window)
	{
		for (int i = 0; i < WindowCount(); i++)
		{
			if (windowArray[i] == _window)
			{
				DeleteWindow(i);
				return;
			}
		}
	}
}