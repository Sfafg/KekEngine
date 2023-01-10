#pragma once
typedef unsigned int uint;

namespace Kek
{
	namespace Graphics
	{
		class Shader
		{
			uint id;

		public:
			Shader();
			Shader(const char *vertexSource, const char *fragmentSource);
			void Bind();
			void Delete();
		};
	}
}
