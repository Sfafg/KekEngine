#pragma once
#include <iosfwd>
#include "Matrix.h"
#include "Byte.h"

class Transform2D
{
	public:
	vec2f position;
	vec2f scale;
	float rotation;
	mat3x3 matrix;

	Transform2D(vec2f position = { 0,0 }, vec2f scale = { 1,1 }, float rotation = 0);

	void Apply();
	vec2f Right();
	vec2f Up();
};
std::ostream& operator<<(std::ostream& os, const Transform2D& tr);

class Transform3D
{
	public:
	vec3f position;
	vec3f scale;
	vec3f rotation;
	mat4x4 matrix;

	Transform3D(vec3f position = { 0,0,0 }, vec3f scale = { 1,1,1 }, vec3f rotation = { 0,0,0 });

	void Apply();
	vec3f Right();
	vec3f Up();
};
std::ostream& operator<<(std::ostream& os, const Transform3D& tr);