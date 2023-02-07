#include "Matrix.h"
#include <iostream>
namespace Kek
{
	mat4x4::mat4x4() {}
	mat4x4::mat4x4(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4, float d1, float d2, float d3, float d4)
		: mat{ a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4 }
	{
	}
	mat4x4::mat4x4(vec4f a, vec4f b, vec4f c, vec4f d)
		: mat(vec<4, vec<4, float>>{ a, b, c, d })
	{
	}

	vec3f mat4x4::TransformVector(vec4f a) const
	{
		a.w = 1;
		return (vec<4>)(*this * mat<4, 1>(a));
	}
	vec3f mat4x4::TransformDir(vec4f a) const
	{
		a.w = 0;
		return (vec<4>)(*this * mat<4, 1>(a));
	}

	void mat4x4::Scale(float x, float y, float z)
	{
		mat4x4 scaleMat
		{
			x,0,0,0,
			0,y,0,0,
			0,0,z,0,
			0,0,0,1
		};
		*this = scaleMat * *this;
	}
	void mat4x4::Scale(const vec3f& a) { Scale(a.x, a.y, a.z); }
	void mat4x4::Translate(float x, float y, float z)
	{
		mat4x4 posMat
		{
			1,0,0,x,
			0,1,0,y,
			0,0,1,z,
			0,0,0,1
		};
		*this = posMat * *this;
	}
	void mat4x4::Translate(const vec3f& a) { Translate(a.x, a.y, a.z); }
	void mat4x4::RotateX(float a)
	{
		if(a == 0)return;
		a = a * Maths::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		mat4x4 rotMat(
			1, 0, 0, 0,
			0, Cos, -Sin, 0,
			0, Sin, Cos, 0,
			0, 0, 0, 1
		);
		*this = rotMat * *this;
	}
	void mat4x4::RotateY(float a)
	{
		if(a == 0)return;
		a = a * Maths::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		mat4x4 rotMat(
			Cos, 0, Sin, 0,
			0, 1, 0, 0,
			-Sin, 0, Cos, 0,
			0, 0, 0, 1
		);
		*this = rotMat * *this;
	}
	void mat4x4::RotateZ(float a)
	{
		if(a == 0)return;
		a = a * Maths::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		mat4x4 rotMat(
			Cos, -Sin, 0, 0,
			Sin, Cos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
		*this = rotMat * *this;
	}
	void mat4x4::Rotate(float x, float y, float z) { RotateZ(z);  RotateY(y); RotateX(x); }
	void mat4x4::Rotate(const vec3f& a) { Rotate(a.x, a.y, a.z); }

	void mat4x4::Transform(const vec3f& pos, const vec3f& scale, const vec3f& rot) { Rotate(rot); Scale(scale); Translate(pos); }
	void mat4x4::InverseTransform(const vec3f& pos, const vec3f& scale, const vec3f& rot) { Translate(-pos);  Rotate(-rot); Scale(vec3f(1, 1, 1) / scale); }

	mat3x3::mat3x3() {}
	mat3x3::mat3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
		: mat{ a1,a2,a3,b1,b2,b3,c1,c2,c3 }
	{
	}
	mat3x3::mat3x3(vec3f a, vec3f b, vec3f c, vec3f d)
		: mat(vec<3, vec<3, float>>{ a, b, c, d })
	{
	}

	vec2f mat3x3::TransformVector(vec3f a) const
	{
		a.z = 1;
		return (vec<3>)(*this * mat<3>(a));
	}
	vec2f mat3x3::TransformDir(vec3f a) const
	{
		a.z = 0;
		return (vec<3>)(*this * mat<3>(a));
	}

	void mat3x3::Scale(float x, float y)
	{
		mat3x3 scaleMat
		{
			x,0,0,
			0,y,0,
			0,0,1
		};
		*this = scaleMat * *this;
	}
	void mat3x3::Scale(const vec2f& a) { Scale(a.x, a.y); }
	void mat3x3::Translate(float x, float y)
	{
		mat3x3 posMat
		{
			1,0,x,
			0,1,y,
			0,0,1
		};
		*this = posMat * *this;
	}
	void mat3x3::Translate(const vec2f& a) { Translate(a.x, a.y); }
	void mat3x3::Rotate(float a)
	{
		if(a == 0) return;
		a = a * Maths::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		mat3x3 rotMat(
			Cos, -Sin, 0,
			Sin, Cos, 0,
			0, 0, 1
		);
		*this = rotMat * *this;
	}

	void mat3x3::Transform(const vec2f& pos, const vec2f& scale, float rot) { Scale(scale); Rotate(rot);  Translate(pos); }
	void mat3x3::InverseTransform(const vec2f& pos, const vec2f& scale, float rot) { Translate(-pos);  Rotate(-rot); Scale(vec2f(1, 1) / scale); }

	mat3x3 Matrix::Pivot(vec2f pivot, vec2f scale, float rotation)
	{
		mat3x3 mat = mat3x3();
		mat.Translate(-pivot);
		mat.Scale(scale);
		mat.Rotate(rotation);
		mat.Translate(pivot);
		return mat;
	}
	mat4x4 Matrix::Pivot(vec3f pivot, vec3f scale, vec3f rotation)
	{
		mat4x4 mat = mat4x4();
		mat.Translate(-pivot);
		mat.Rotate(scale);
		mat.Rotate(rotation);
		mat.Translate(pivot);
		return mat;
	}
	mat4x4 Matrix::LookAt(vec3f position, vec3f target, vec3f up)
	{
		vec3f d = Vector::Normalize(position - target);
		vec3f r = Vector::Normalize(Vector::Cross(up, d));
		vec3f u = Vector::Cross(d, r);

		mat4x4 lookAt(
			r.x, r.y, r.z, -Vector::Dot(r, position),
			u.x, u.y, u.z, -Vector::Dot(u, position),
			d.x, d.y, d.z, -Vector::Dot(d, position),
			0, 0, 0, 1
		);
		return lookAt;
	}
	mat4x4 Matrix::Perspective(float ratio, float fov, float near, float far)
	{
		float s = 1 / (tan(fov / 2 * 3.14159265359f / 180.0f));
		return mat4x4(
			1 / s * ratio, 0, 0, 0,
			0, s, 0, 0,
			0, 0, -far / (far - near), -(far * near) / (far - near),
			0, 0, -1, 0
		);
	}
	mat4x4 Matrix::Orthograpic(float left, float right, float bottom, float top, float near, float far)
	{
		return mat4x4(
			2 / (right - left), 0, 0, -(right + left) / (right - left),
			0, -2 / (top - bottom), 0, (top + bottom) / (top - bottom),
			0, 0, 2 / (far - near), -(far + near) / (far - near),
			0, 0, 0, 1
		);
	}
}