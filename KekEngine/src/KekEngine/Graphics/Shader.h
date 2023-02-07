#pragma once
#include <string>
#include <typeinfo>
#include <initializer_list>
#include "KekEngine/Maths/Vector.h"
#include "KekEngine/Maths/Matrix.h"
namespace Kek
{
	namespace Graphics
	{
		typedef unsigned int uint;

		namespace SHADER
		{
			namespace SOURCE
			{
				inline uint TEXT = 1;
				inline uint SHDR = 2;
				inline uint FILE = 4;
			};

			extern uint VERTEX;
			extern uint GEOMETRY;
			extern uint FRAGMENT;
			extern uint COMPUTE;
		};

		struct SourceData
		{
			const char *source;
			uint sourceEnum;
			uint type;
		};

		class Shader
		{
			uint id;

			bool Link(std::string *linkingLog = nullptr);

		public:
			Shader();
			Shader(std::initializer_list<SourceData> shaderSources);

			int GetUniformLocation(const char *name);

			void SetUniform(int location, const float *value, uint count = 1);
			void SetUniform(int location, const int *value, uint count = 1);
			void SetUniform(int location, const unsigned int *value, uint count = 1);

			void SetUniform(int location, const vec<2, float> *value, uint count = 1);
			void SetUniform(int location, const vec<3, float> *value, uint count = 1);
			void SetUniform(int location, const vec<4, float> *value, uint count = 1);
			void SetUniform(int location, const vec<2, int> *value, uint count = 1);
			void SetUniform(int location, const vec<3, int> *value, uint count = 1);
			void SetUniform(int location, const vec<4, int> *value, uint count = 1);
			void SetUniform(int location, const vec<2, uint> *value, uint count = 1);
			void SetUniform(int location, const vec<3, uint> *value, uint count = 1);
			void SetUniform(int location, const vec<4, uint> *value, uint count = 1);

			void SetUniform(int location, const mat<2, 2, float> *value, uint count = 1);
			void SetUniform(int location, const mat<3, 3, float> *value, uint count = 1);
			void SetUniform(int location, const mat<4, 4, float> *value, uint count = 1);

			template <typename T>
			void SetUniform(const char *name, T *value, uint count = 1)
			{
				SetUniform(GetUniformLocation(name), value, count);
			}
			void Bind();
			void Delete();
		};
	};
};