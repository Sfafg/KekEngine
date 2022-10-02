#include "Monitor.h"
#include "GLFW/glfw3.h"

namespace Kek
{
	namespace Monitors
	{
		Monitor* GetArray()
		{
			int size;
			return (Monitor*)glfwGetMonitors(&size);
		}
		Monitor* GetArray(int& size)
		{
			return(Monitor*)glfwGetMonitors(&size);
		}
		Monitor Get(int index)
		{
			int size;
			Monitor* arr = GetArray(size);
			if(index >= size) return Monitor();
			return arr[index];
		}
	};

	Monitor::Monitor() : glfwMonitor(NULL) {}
	Monitor::Monitor(int index) : glfwMonitor(Monitors::Get(index)) { glfwSetMonitorUserPointer(glfwMonitor, this); }

	const char* Monitor::Name() const
	{
		if(glfwMonitor == NULL) return "NULL";
		return glfwGetMonitorName(glfwMonitor);
	}
	vec2i Monitor::Size() const
	{
		if(glfwMonitor == NULL) return vec2i(0, 0);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwMonitor);
		return vec2i(mode->width, mode->height);
	}
	int Monitor::RefreshRate() const
	{
		if(glfwMonitor == NULL) return -1;
		const GLFWvidmode* mode = glfwGetVideoMode(glfwMonitor);
		return mode->refreshRate;
	}
	vec2i Monitor::PhisicalSize() const
	{
		if(glfwMonitor == NULL) return vec2i(0, 0);
		vec2i a; glfwGetMonitorPhysicalSize(glfwMonitor, &a.x, &a.y); return a;
	}
	vec2i Monitor::Position() const
	{
		if(glfwMonitor == NULL) return vec2i(-1, -1);
		vec2i a; glfwGetMonitorPos(glfwMonitor, &a.x, &a.y); return a;
	}

	Monitor::operator GLFWmonitor* ()
	{
		return glfwMonitor;
	}
}