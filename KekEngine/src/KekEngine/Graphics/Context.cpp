#include "Context.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "KekEngine/Core/Log.h"

namespace Kek
{
	void error_callback(int error, const char* description)
	{
		Log<Error>(description);
	}
	struct GlobalInit
	{
		GlobalInit()
		{
			glfwSetErrorCallback(error_callback);
		}
	}globalInit;

	namespace SystemContext
	{
		bool initialized = false;
		bool Init()
		{
			if(initialized) return true;
			if(glfwInit() == GLFW_FALSE)
			{
				Log<Error>("Failed to initialize GLFW.");
				return false;
			}
			Log<Info>("Initalized GLFW.");
			initialized = true;

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
		bool initialized = false;
		bool Init(int majorVersion, int minorVersion)
		{
			if(initialized) return true;
			SystemContext::Init();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				Log<Error>("Failed to initialize GLAD.");
				return false;
			}
			Log<Info>("Initalized GLAD.");
			initialized = true;

			return true;
		}
	}

}