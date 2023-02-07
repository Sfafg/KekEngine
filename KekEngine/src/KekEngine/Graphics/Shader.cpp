#include "Shader.h"
#include "glad/glad.h"
#include "KekEngine/Core/Log.h"
namespace Kek
{
	namespace Graphics
	{
		namespace SHADER
		{
			uint VERTEX = GL_VERTEX_SHADER;
			uint GEOMETRY = GL_GEOMETRY_SHADER;
			uint FRAGMENT = GL_FRAGMENT_SHADER;
			uint COMPUTE = GL_COMPUTE_SHADER;
		};

		class ShaderSource
		{
			uint id;

			friend class Shader;

		public:
			ShaderSource(const char *source, uint sourceEnumu, uint shaderType) : id(glCreateShader(shaderType))
			{
				glShaderSource(id, 1, &source, NULL);
			}

			bool Compile(std::string *compilationLog = nullptr)
			{
				glCompileShader(id);

				int logLength = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
				if (logLength == 0 || compilationLog == nullptr)
				{
					return logLength == 0;
				}

				compilationLog->resize(logLength);
				glGetShaderInfoLog(id, logLength, NULL, compilationLog->data());

				return false;
			}
			~ShaderSource()
			{
				glDeleteShader(id);
			}
		};

		Shader::Shader()
		{
			id = glCreateProgram();
		}
		Shader::Shader(std::initializer_list<SourceData> sourceData) : Shader()
		{
			for (int i = 0; i < sourceData.size(); i++)
			{
				const SourceData &data = sourceData.begin()[i];
				ShaderSource source(data.source, data.sourceEnum, data.type);

				std::string log;
				if (!source.Compile(&log))
				{
					Log<ERROR>("Failed To Compile Shader.\n", log);
				}

				glAttachShader(id, source.id);
			}

			std::string log;
			if (!Link(&log))
			{
				Log<ERROR>("Failed To Link Shader.\n", log);
			}
			Bind();
		}
		bool Shader::Link(std::string *linkingLog)
		{
			glLinkProgram(id);

			int logLength = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength == 0 || linkingLog == nullptr)
			{
				return logLength == 0;
			}

			linkingLog->resize(logLength);
			glGetShaderInfoLog(id, logLength, NULL, linkingLog->data());

			Bind();
			return false;
		}

		int Shader::GetUniformLocation(const char *name)
		{
			return glGetUniformLocation(id, name);
		}

		void Shader::SetUniform(int location, const float *value, uint count) { glUniform1fv(location, count, value); }
		void Shader::SetUniform(int location, const int *value, uint count) { glUniform1iv(location, count, value); }
		void Shader::SetUniform(int location, const unsigned int *value, uint count) { glUniform1uiv(location, count, value); }
		void Shader::SetUniform(int location, const vec<2, float> *value, uint count) { glUniform2fv(location, count, (float *)value); }
		void Shader::SetUniform(int location, const vec<3, float> *value, uint count) { glUniform3fv(location, count, (float *)value); }
		void Shader::SetUniform(int location, const vec<4, float> *value, uint count) { glUniform4fv(location, count, (float *)value); }
		void Shader::SetUniform(int location, const vec<2, int> *value, uint count) { glUniform2iv(location, count, (int *)value); }
		void Shader::SetUniform(int location, const vec<3, int> *value, uint count) { glUniform3iv(location, count, (int *)value); }
		void Shader::SetUniform(int location, const vec<4, int> *value, uint count) { glUniform4iv(location, count, (int *)value); }
		void Shader::SetUniform(int location, const vec<2, uint> *value, uint count) { glUniform2uiv(location, count, (uint *)value); }
		void Shader::SetUniform(int location, const vec<3, uint> *value, uint count) { glUniform3uiv(location, count, (uint *)value); }
		void Shader::SetUniform(int location, const vec<4, uint> *value, uint count) { glUniform4uiv(location, count, (uint *)value); }
		void Shader::SetUniform(int location, const mat<2, 2, float> *value, uint count) { glUniformMatrix2fv(location, count, false, (float *)value); }
		void Shader::SetUniform(int location, const mat<3, 3, float> *value, uint count) { glUniformMatrix3fv(location, count, false, (float *)value); }
		void Shader::SetUniform(int location, const mat<4, 4, float> *value, uint count) { glUniformMatrix4fv(location, count, false, (float *)value); }

		void Shader::Bind()
		{
			glUseProgram(id);
		}
		void Shader::Delete()
		{
			glDeleteProgram(id);
		}
	};
};