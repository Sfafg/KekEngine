#include "Context.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "KekEngine/Core/Log.h"

namespace Kek
{
	namespace WindowContext
	{
		bool Init(int majorVersion, int minorVersion)
		{
			if(glfwInit() == GLFW_FALSE)
			{
				Log<Error>("Failed to initialize GLFW.");
				return false;
			}
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			Log<Info>("Initalized GLFW.");
			return true;
		}
		void Terminate()
		{
			glfwTerminate();
			Log<Info>("Terminated GLFW.");
		}
	}
	namespace GraphicsContext
	{
		bool Init()
		{
			if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				Log<Error>("Failed to initialize GLAD.");
				return false;
			}
			Log<Info>("Initalized GLAD.");
			return true;
		}
	}

}