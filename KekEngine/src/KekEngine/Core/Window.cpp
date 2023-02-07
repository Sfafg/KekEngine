#include "Window.h"
#include "GLFW/glfw3.h"
#include "KekEngine/Core/Log.h"
#include "KekEngine/Maths/Byte.h"
#include "GLFW_Keys.h"

namespace Kek
{
	Window::Window() : name(nullptr), window(nullptr) {}
	Window::Window(const char *title, vec2i size, FlagSet style, Monitor monitor) : name(title)
	{
		glfwDefaultWindowHints();

		if (style.IsUp(Fullscreen))
		{
			if (monitor == nullptr)
			{
				monitor = Monitors::Primary();
			}
			size = monitor.Size();
		}
		glfwWindowHint(GLFW_DECORATED, style.IsDown(Borderless));
		glfwWindowHint(GLFW_FLOATING, style.IsUp(Floating));
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, style.IsUp(Transparent));

		window = glfwCreateWindow(size.x, size.y, title, (GLFWmonitor *)(void *)monitor, NULL);
		if (window == NULL)
		{
			Log<ERROR>("Failed to Create Window ", title, '.');
			return;
		}
		ContextCurrent();

		glfwSetWindowUserPointer((GLFWwindow *)window, (void *)this);
		glfwSetFramebufferSizeCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, int w, int h)
			{
				glViewport(0, 0, w, h);

				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnResize(vec2i(w, h), win);
			});
		glfwSetWindowCloseCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnClose(win);
			});
		glfwSetWindowFocusCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, int focused)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnFocus(focused == GLFW_TRUE, win);
			});
		glfwSetWindowMaximizeCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, int maximized)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnMaximize(maximized == GLFW_TRUE, win);
			});
		glfwSetWindowPosCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, int x, int y)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnMove(vec2i(x, y), win);
			});
		glfwSetCursorEnterCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, int entered)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnMouseEnter(entered == GLFW_TRUE, win);
			});
		glfwSetCursorPosCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, double x, double y)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnMouseMove(vec2i(x, y), win);
			});
		glfwSetKeyCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, int key, int scancode, int action, int mods)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnKey(GLFWToKey(key), scancode, GLFWToState(action), mods, win);
			});
		glfwSetMouseButtonCallback(
			(GLFWwindow *)window,
			[](GLFWwindow *window, int button, int action, int mods)
			{
				Window *win = (Window *)glfwGetWindowUserPointer(window);
				win->OnKey(GLFWToKey(button), GLFWToKey(button), GLFWToState(action), mods, win);
			});
	}
	Window::Window(const char *title, vec2f size, FlagSet style, Monitor monitor) : Window(title, vec2i(size * monitor.Size()), style, monitor) {}
	Window::Window(const char *title, FlagSet style, Monitor monitor) : Window(title, vec2i(0, 0), style, monitor) {}

	Window::Window(const Window &o) : name(o.name), window(o.window), OnMouseMove(o.OnMouseMove), OnMove(o.OnMove), OnMouseEnter(o.OnMouseEnter), OnResize(o.OnResize), OnClose(o.OnClose), OnMaximize(o.OnMaximize), OnFocus(o.OnFocus)
	{
		glfwSetWindowUserPointer((GLFWwindow *)window, (void *)this);
	}
	void Window::operator=(const Window &o)
	{
		name = o.name;
		window = o.window;
		glfwSetWindowUserPointer((GLFWwindow *)window, (void *)this);

		OnMouseMove = o.OnMouseMove;
		OnMove = o.OnMove;
		OnMouseEnter = o.OnMouseEnter;
		OnResize = o.OnResize;
		OnClose = o.OnClose;
		OnMaximize = o.OnMaximize;
		OnFocus = o.OnFocus;
	}

	Window::operator void *()
	{
		return window;
	}

	const char *Window::Title() const
	{
		return name;
	}
	void Window::SetTitle(const char *title)
	{
		name = title;
		glfwSetWindowTitle((GLFWwindow *)window, name);
	}

	vec2i Window::Position() const
	{
		vec2i pos;
		glfwGetWindowPos((GLFWwindow *)window, &pos.x, &pos.y);
		return pos;
	}
	void Window::SetPosition(vec2i pos)
	{
		glfwSetWindowPos((GLFWwindow *)window, pos.x, pos.y);
	}
	vec2i Window::Size() const
	{
		vec2i size;
		glfwGetWindowSize((GLFWwindow *)window, &size.x, &size.y);
		return size;
	}
	void Window::SetSize(vec2i size)
	{
		glfwSetWindowSize((GLFWwindow *)window, size.x, size.y);
	}
	float Window::Opacity()
	{
		return glfwGetWindowOpacity((GLFWwindow*)window);
	}
	void Window::SetOpacity(float opacity)
	{
		glfwSetWindowOpacity((GLFWwindow*)window, opacity);
	}
	vec2f Window::MousePosition() const
	{
		double x, y;
		glfwGetCursorPos((GLFWwindow *)window, &x, &y);
		return vec2f(x, y);
	}
	void Window::SetMousePosition(vec2f mpos)
	{
		glfwSetCursorPos((GLFWwindow *)window, mpos.x, mpos.y);
	}
	bool Window::GetKey(int key) const
	{
		return glfwGetKey((GLFWwindow *)window, KeyToGLFW(key)) == GLFW_PRESS;
	}
	bool Window::GetButton(int button)
	{
		return glfwGetMouseButton((GLFWwindow *)window, KeyToGLFW(button)) == GLFW_PRESS;
	}

	void Window::Focus()
	{
		glfwFocusWindow((GLFWwindow *)window);
	}
	Monitor Window::GetMonitor()
	{
		GLFWmonitor *ptr = glfwGetWindowMonitor((GLFWwindow *)window);
		if (ptr == NULL)
			return Monitor();
		return *(Monitor *)ptr;
	}
	void Window::SetMonitor(Monitor monitor)
	{
		vec2i pos = Position();
		vec2i size = Size();
		glfwSetWindowMonitor((GLFWwindow *)window, (GLFWmonitor *)(void *)monitor, pos.x, pos.y, size.x, size.y, monitor.RefreshRate());
	}

	void Window::Clear(vec4f col)
	{
		glClearColor(col.x, col.y, col.z, col.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Window::SwapBuffers()
	{
		glfwSwapBuffers((GLFWwindow *)window);
	}
	void Window::ContextCurrent()
	{
		glfwMakeContextCurrent((GLFWwindow *)window);
	}
	void Window::Close()
	{
		glfwSetWindowShouldClose((GLFWwindow *)window, GLFW_TRUE);
	}
	bool Window::Closed() const
	{
		return glfwWindowShouldClose((GLFWwindow *)window);
	}
	void Window::Destroy()
	{
		glfwDestroyWindow((GLFWwindow *)window);
	}
}
