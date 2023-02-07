#include "Monitor.h"
#include "GLFW/glfw3.h"

namespace Kek
{
	namespace Monitors
	{
		Monitor* GetAll()
		{
			int size;
			return (Monitor*)glfwGetMonitors(&size);
		}
		Monitor* GetAll(int& size)
		{
			return(Monitor*)glfwGetMonitors(&size);
		}
		Monitor Get(int index)
		{
			int size;
			Monitor* arr = GetAll(size);
			if(index >= size) return Monitor();
			return arr[index];
		}
		Monitor Primary()
		{
			return Monitor(glfwGetPrimaryMonitor());
		}
	};

	Monitor::Monitor(void* monitor) : monitor(monitor) {}
	Monitor::Monitor(int index) : monitor(Monitors::Get(index)) { glfwSetMonitorUserPointer((GLFWmonitor*)monitor, this); }

	Monitor::Monitor(const Monitor& o)
	{
		monitor = o.monitor;
		if(monitor != NULL)glfwSetMonitorUserPointer((GLFWmonitor*)monitor, this);
	}

	Monitor::operator void* ()
	{
		return monitor;
	}

	const char* Monitor::Name() const
	{
		if(monitor == NULL) return "NULL";
		return glfwGetMonitorName((GLFWmonitor*)monitor);
	}
	vec2i Monitor::Size() const
	{
		if(monitor == NULL) return vec2i(0, 0);
		const GLFWvidmode* mode = glfwGetVideoMode((GLFWmonitor*)monitor);
		return vec2i(mode->width, mode->height);
	}
	int Monitor::RefreshRate() const
	{
		if(monitor == NULL) return -1;
		const GLFWvidmode* mode = glfwGetVideoMode((GLFWmonitor*)monitor);
		return mode->refreshRate;
	}
	vec2i Monitor::PhisicalSize() const
	{
		if(monitor == NULL) return vec2i(0, 0);
		vec2i a; glfwGetMonitorPhysicalSize((GLFWmonitor*)monitor, &a.x, &a.y); return a;
	}
	vec2i Monitor::Position() const
	{
		if(monitor == NULL) return vec2i(-1, -1);
		vec2i a; glfwGetMonitorPos((GLFWmonitor*)monitor, &a.x, &a.y); return a;
	}
}