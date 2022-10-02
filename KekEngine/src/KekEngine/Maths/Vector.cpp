#include "Vector.h"
namespace Kek
{
	vec2f::vec2f() {}
	vec2f::vec2f(float x, float y) : vec{ x,y } {}

	vec3f::vec3f() {}
	vec3f::vec3f(float x, float y, float z) : vec{ x,y,z } {}

	vec4f::vec4f() {}
	vec4f::vec4f(float x, float y, float z, float w) : vec{ x,y,z,w } {}


	vec2i::vec2i() {}
	vec2i::vec2i(int x, int y) : vec{ x,y } {}

	vec3i::vec3i() {}
	vec3i::vec3i(int x, int y, int z) : vec{ x,y,z } {}

	vec4i::vec4i() {}
	vec4i::vec4i(int x, int y, int z, int w) : vec{ x,y,z,w } {}
}