#include "Window.h"
#include "GLFW/glfw3.h"
#include "KekEngine/Core/Log.h"
#include "KekEngine/Maths/Byte.h"

namespace Kek
{
	Window::Window(const char* title, FlagSet style, vec2i size, vec2i pos, Monitor monitor)
	{
		if(style.IsUp(WindowStyle::Fullscreen))
		{
			if(monitor == NULL) monitor = Monitor(0);
			size = monitor.Size();
			pos = vec2i(0, 0);
		}
		if(style.IsUp(WindowStyle::Borderless))
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			monitor = Monitor();
		}
		if(style.IsUp(WindowStyle::Floating))
		{
			glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		}

		glfwWindow = glfwCreateWindow(size.x, size.y, title, monitor, NULL);
		if(glfwWindow == NULL)
		{
			Log<Error>("Failed to Create Window ", title, '.');
			return;
		}
		ContextCurrent();

		glfwSetWindowPos(glfwWindow, pos.x, pos.y);
		glfwSetWindowUserPointer(glfwWindow, this);
		glfwSetFramebufferSizeCallback
		(
			glfwWindow,
			[](GLFWwindow* window, int w, int h)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->OnResize(vec2i(w, h), win);
			}
		);
		glfwSetWindowCloseCallback
		(
			glfwWindow,
			[](GLFWwindow* window)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->OnClose(win);
			}
		);
		glfwSetWindowFocusCallback
		(
			glfwWindow,
			[](GLFWwindow* window, int focused)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->OnFocus(focused == GLFW_TRUE, win);
			}
		);
		glfwSetWindowMaximizeCallback
		(
			glfwWindow,
			[](GLFWwindow* window, int maximized)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->OnMaximize(maximized == GLFW_TRUE, win);
			}
		);
		glfwSetWindowPosCallback
		(
			glfwWindow,
			[](GLFWwindow* window, int x, int y)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->OnMove(vec2i(x, y), win);
			}
		);
		glfwSetCursorEnterCallback
		(
			glfwWindow,
			[](GLFWwindow* window, int entered)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->OnMouseEnter(entered == GLFW_TRUE, win);
			}
		);
		glfwSetCursorPosCallback
		(
			glfwWindow,
			[](GLFWwindow* window, double x, double y)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->OnMouseMove(vec2i(x, y), win);
			}
		);

		Log<Info>("Created Window ", title, '.');
	}
	Window::Window(const char* title, FlagSet style, vec2f size, vec2f pos, Monitor monitor) : Window(title, style, vec2i(size* Monitor(0).Size()), vec2i(pos* Monitor(0).Size()), monitor)
	{
	}
	Window::Window(const char* title, vec2i size, vec2i pos, Monitor monitor) : Window(title, 0, size, pos, monitor)
	{
	}
	Window::Window(const char* title, vec2f size, vec2f pos, Monitor monitor) : Window(title, 0, vec2i(size* Monitor(0).Size()), vec2i(pos* Monitor(0).Size()), monitor)
	{
	}

	Window::Window(const char* title, FlagSet style, Monitor monitor) : Window(title, style, monitor.Size())
	{
	}
	Window::operator GLFWwindow* ()
	{
		return glfwWindow;
	}

	vec2i Window::Position()
	{
		vec2i pos;
		glfwGetWindowPos(glfwWindow, &pos.x, &pos.y);
		return pos;
	}
	void Window::SetPosition(vec2i pos)
	{
		glfwSetWindowSize(glfwWindow, pos.x, pos.y);
	}
	vec2i Window::Size()
	{
		vec2i size;
		glfwGetWindowSize(glfwWindow, &size.x, &size.y);
		return size;
	}
	void Window::SetSize(vec2i size)
	{
		glfwSetWindowSize(glfwWindow, size.x, size.y);
	}

	vec2f Window::MousePosition()
	{
		vec2f a;
		glfwGetCursorPos(glfwWindow, (double*)&a.x, (double*)&a.y);
		return a;
	}
	void Window::SetMousePosition(vec2f mpos)
	{
		glfwSetCursorPos(glfwWindow, mpos.x, mpos.y);
	}
	bool Window::GetKey(int key)
	{
		return glfwGetKey(glfwWindow, key) == GLFW_PRESS;
	}
	bool Window::GetButton(int button)
	{
		return glfwGetMouseButton(glfwWindow, button) == GLFW_PRESS;
	}

	void Window::Focus()
	{
		glfwFocusWindow(glfwWindow);
	}
	Monitor Window::GetMonitor()
	{
		GLFWmonitor* ptr = glfwGetWindowMonitor(glfwWindow);
		if(ptr == NULL) return Monitor();
		return *(Monitor*)ptr;
	}
	void Window::SetMonitor(Monitor monitor)
	{
		vec2i pos = Position();
		vec2i size = Size();
		glfwSetWindowMonitor(glfwWindow, monitor, pos.x, pos.y, size.x, size.y, monitor.RefreshRate());
	}

	void Window::SetAttribute(int attrib, int value)
	{
		glfwSetWindowAttrib(glfwWindow, attrib, value);
	}
	void Window::ContextCurrent()
	{
		glfwMakeContextCurrent(glfwWindow);
	}
	void Window::Close()
	{
		glfwSetWindowShouldClose(glfwWindow, true);
	}
	bool Window::Closed()
	{
		return glfwWindowShouldClose(glfwWindow);
	}
	void Window::Destroy()
	{
		glfwDestroyWindow(glfwWindow);
	}
}
