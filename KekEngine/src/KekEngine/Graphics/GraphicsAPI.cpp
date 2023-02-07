#include "GraphicsAPI.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "KekEngine/Core/Log.h"
typedef unsigned int uint;

namespace Kek
{

	namespace Graphics
	{
		void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam)
		{
			// ignore non-significant error/warning codes
			if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
				return;

			Log("---------------\n", "Debug message (", id, "): ", message);

			switch (source)
			{
			case GL_DEBUG_SOURCE_API:
				Log("Source: API");
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				Log("Source: Window System");
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				Log("Source: Shader Compiler");
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				Log("Source: Third Party");
				break;
			case GL_DEBUG_SOURCE_APPLICATION:
				Log("Source: Application");
				break;
			case GL_DEBUG_SOURCE_OTHER:
				Log("Source: Other");
				break;
			}

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:
				Log("Type: Error");
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				Log("Type: Deprecated Behaviour");
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				Log("Type: Undefined Behaviour");
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				Log("Type: Portability");
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				Log("Type: Performance");
				break;
			case GL_DEBUG_TYPE_MARKER:
				Log("Type: Marker");
				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				Log("Type: Push Group");
				break;
			case GL_DEBUG_TYPE_POP_GROUP:
				Log("Type: Pop Group");
				break;
			case GL_DEBUG_TYPE_OTHER:
				Log("Type: Other");
				break;
			}

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				Log("Severity: high");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				Log("Severity: medium");
				break;
			case GL_DEBUG_SEVERITY_LOW:
				Log("Severity: low");
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				Log("Severity: notification");
				break;
			}
			Log();
		}
		bool Initialize()
		{
#ifndef NDEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, 0);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
			return true;
		}

		void GetError()
		{
			GLenum errorCode;
			while ((errorCode = glGetError()) != GL_NO_ERROR)
			{
				std::string error;
				switch (errorCode)
				{
				case GL_INVALID_ENUM:
					error = "INVALID_ENUM";
					break;
				case GL_INVALID_VALUE:
					error = "INVALID_VALUE";
					break;
				case GL_INVALID_OPERATION:
					error = "INVALID_OPERATION";
					break;
				case GL_STACK_OVERFLOW:
					error = "STACK_OVERFLOW";
					break;
				case GL_STACK_UNDERFLOW:
					error = "STACK_UNDERFLOW";
					break;
				case GL_OUT_OF_MEMORY:
					error = "OUT_OF_MEMORY";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					error = "INVALID_FRAMEBUFFER_OPERATION";
					break;
				}
				Log<ERROR>(error);
			}
		}
	}
}