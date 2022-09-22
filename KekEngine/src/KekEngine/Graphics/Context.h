#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "Logging/Log.h"

namespace OpenGL
{
	bool Initialize(int majorVersion, int minorVersion)
	{
		if(glfwInit() == GLFW_FALSE)
		{
			//Log<Error>("Failed to initialize OpenGL Context.");
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		return true;
		//if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		//{
			//Log<Error>("Failed to initialize GLAD.");
		//	return false;
		//}

	}
}