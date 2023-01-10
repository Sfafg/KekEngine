#include "Vector.h"
namespace Kek
{
	vec2f::vec2f() {}
	vec2f::vec2f(float x, float y) : vec{ x,y } {}
	vec2f::vec2f(std::initializer_list<float> data){ memcpy(this->value, data.begin(), sizeof(float) * 2); }

	vec3f::vec3f() {}
	vec3f::vec3f(float x, float y, float z) : vec{ x,y,z } {}
	vec3f::vec3f(std::initializer_list<float> data) { memcpy(this->value, data.begin(), sizeof(float) * 3); }

	vec4f::vec4f() {}
	vec4f::vec4f(float x, float y, float z, float w) : vec{ x,y,z,w } {}
	vec4f::vec4f(std::initializer_list<float> data) { memcpy(this->value, data.begin(), sizeof(float) * 4); }


	vec2i::vec2i() {}
	vec2i::vec2i(int x, int y) : vec{ x,y } {}
	vec2i::vec2i(std::initializer_list<int> data) { memcpy(this->value, data.begin(), sizeof(int) * 2); }

	vec3i::vec3i() {}
	vec3i::vec3i(int x, int y, int z) : vec{ x,y,z } {}
	vec3i::vec3i(std::initializer_list<int> data) { memcpy(this->value, data.begin(), sizeof(int) * 3); }

	vec4i::vec4i() {}
	vec4i::vec4i(int x, int y, int z, int w) : vec{ x,y,z,w } {}
	vec4i::vec4i(std::initializer_list<int> data) { memcpy(this->value, data.begin(), sizeof(int) * 4); }
}