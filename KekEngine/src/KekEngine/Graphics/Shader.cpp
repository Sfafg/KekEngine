#include "Shader.h"
#include "glad/glad.h"
#include "KekEngine/Core/Log.h"
namespace Kek
{
	namespace Graphics
	{
		uint CreateShader(uint type, const char *src)
		{
			uint id = glCreateShader(type);
			glShaderSource(id, 1, &src, nullptr);

			return id;
		}

		bool CompileShader(uint id, char *&compilationLog)
		{
			glCompileShader(id);
			uint infoLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, (GLint *)&infoLength);
			if (infoLength != 0)
			{
				compilationLog = new char[infoLength];
				glGetShaderInfoLog(id, infoLength, nullptr, compilationLog);

				return false;
			}
			return true;
		}
		void AttachShader(uint programID, uint shaderID)
		{
			glAttachShader(programID, shaderID);
		}
		void DeleteShader(uint id)
		{
			glDeleteShader(id);
		}

		Shader::Shader()
		{
			id = glCreateProgram();
		}
		Shader::Shader(const char *vertexShader, const char *fragmentShader) : Shader()
		{
			uint vertexID = CreateShader(GL_VERTEX_SHADER, vertexShader);
			uint fragmentID = CreateShader(GL_FRAGMENT_SHADER, fragmentShader);

			char *compilationLog = nullptr;
			if (!CompileShader(vertexID, compilationLog))
			{
				Log<ERROR>("Failed To Compile VERTEX SHADER.");
				Log<ERROR>(compilationLog);
				delete[] compilationLog;
			}
			if (!CompileShader(fragmentID, compilationLog))
			{
				Log<ERROR>("Failed To Compile FRAGMENT SHADER.");
				Log<ERROR>(compilationLog);
				delete[] compilationLog;
			}

			AttachShader(id, vertexID);
			AttachShader(id, fragmentID);
			glLinkProgram(id);

			uint length;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, (GLint *)&length);

			compilationLog = new char[length];
			if (length != 0)
			{
				glGetShaderInfoLog(id, length, NULL, compilationLog);
				Log<ERROR>("Failed to Link Program:\n");
				Log<ERROR>(compilationLog);
				delete[] compilationLog;
			};

			DeleteShader(vertexID);
			DeleteShader(fragmentID);

			Bind();
		}

		void Shader::Bind()
		{
			glUseProgram(id);
		}
		void Shader::Delete()
		{
			glDeleteProgram(id);
		}
	}
}