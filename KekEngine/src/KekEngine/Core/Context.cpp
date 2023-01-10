#include "Context.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Log.h"
#include "KekEngine/Graphics/GraphicsAPI.h"

namespace Kek
{
	void GLFW_ERROR_CALLBACK(int error, const char *description)
	{
		Log<ERROR>(description);
	}

	namespace SystemContext
	{
		bool initialized = false;
		bool Initialize()
		{
			if (initialized)
				return true;

			glfwSetErrorCallback(GLFW_ERROR_CALLBACK);

			if (glfwInit() == GLFW_FALSE)
			{
				Log<ERROR>("Failed to initialize GLFW.");
				return false;
			}
			Log<INFO>("Initialized GLFW");

			initialized = true;

			return true;
		}
		void Terminate()
		{
			glfwTerminate();
			Log<INFO>("Terminated GLFW.");
		}
	}
	namespace GraphicsContext
	{
		bool initialized = false;
		bool Initialize()
		{
			if (initialized)
				return true;
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				Log<ERROR>("Failed to initialize GLAD.");
				return false;
			}
			Log<INFO>("Initialized GLAD");

			if (!Graphics::Initialize())
			{
				Log<ERROR>("Failed to initialize GraphicsAPI.");
				return false;
			}
			Log<INFO>("Initialized GraphicsAPI");
			initialized = true;

			return true;
		}
	}
}