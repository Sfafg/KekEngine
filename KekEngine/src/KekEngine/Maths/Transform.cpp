#include "Transform.h"
#include <iostream>
namespace Kek
{
	Transform2D::Transform2D(vec2f position, vec2f scale, float rotation)
		: position(position), scale(scale), rotation(rotation), matrix()
	{
		matrix.Transform(position, scale, rotation);
	}

	void Transform2D::Apply()
	{
		matrix = mat3x3();
		matrix.Transform(position, scale, rotation);
	}
	vec2f Transform2D::Right()
	{
		return matrix.TransformDir({ 1,0 });
	}
	vec2f Transform2D::Up()
	{
		return matrix.TransformDir({ 0,1 });
	}

	std::ostream& operator<<(std::ostream& os, const Transform2D& tr)
	{
		os << "[{LCyan}]<Transform2D >\n";
		os << "[4{LYellow}]<";
		os << tr.position << '\n';
		os << tr.scale << '\n';
		os << tr.rotation << '\n';
		os << tr.matrix << '\n';
		os << '>';
		return os;
	}

	Transform3D::Transform3D(vec3f position, vec3f scale, vec3f rotation)
		: position(position), scale(scale), rotation(rotation), matrix()
	{
		matrix.Transform(position, scale, rotation);
	}

	void Transform3D::Apply()
	{
		matrix = mat4x4();
		matrix.Transform(position, scale, rotation);
	}
	vec3f Transform3D::Right()
	{
		return matrix.TransformDir({ 1,0 });
	}
	vec3f Transform3D::Up()
	{
		return matrix.TransformDir({ 0,1 });
	}

	std::ostream& operator<<(std::ostream& os, const Transform3D& tr)
	{
		os << "[{LCyan}]<Transform3D >\n";
		os << "[4{LYellow}]<";
		os << tr.position << '\n';
		os << tr.scale << '\n';
		os << tr.rotation << '\n';
		os << tr.matrix << '\n';
		os << '>';
		return os;
	}
}