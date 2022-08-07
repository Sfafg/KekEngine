#pragma once
#include <iostream>
#include "Vector.h"

template <unsigned int Rows, unsigned int Columns = 1, typename T = float>
struct Matrix
{
	int rows() { return sizeof(value) / sizeof(value[0]); }
	int columns() { return value[0].size(); }

#pragma region OPERATORS
	template <unsigned int ORows, unsigned int OColumns, typename OT>
	void operator =(Matrix<ORows, OColumns, OT> const& o)
	{
		int i, j;
		for(i = 0; i < Math::Min(ORows, Rows); i++)
		{
			for(j = 0; j < Math::Min(OColumns, Columns); j++)
			{
				value[i][j] = o[i][j];
			}
		}
	}
	template <int OColumns>
	Matrix<Rows, OColumns, T>  operator * (Matrix<Columns, OColumns, T> const& o)
	{
		Matrix<Rows, OColumns, T> result;

		for(int i = 0; i < Rows; i++)
		{
			for(int j = 0; j < OColumns; j++)
			{
				result[i][j] = 0;
				for(int k = 0; k < Columns; k++)
				{
					result[i][j] += (*this)[i][k] * o[k][j];
				}
			}
		}

		return result;
	}
	template <int OColumns>
	void operator *= (Matrix<Columns, OColumns, T> const& o)
	{
		*this = *this * o;
	}

	vec<Columns, T>& operator[](int index) { return value[index]; }
	const vec<Columns, T>& operator[](int index) const { return value[index]; }

	T* operator &() { return &this->value[0][0]; }
	explicit operator T* () { return &this->value[0][0]; }
	explicit operator const T* () { return &this->value[0][0]; }
#pragma endregion

	Matrix(T a = 1)
	{
		for(int i = 0; i < Rows; i++)
			for(int j = 0; j < Columns; j++)
				value[i][j] = a * (i == j);
	}
	Matrix(T* data) { memcpy((void*)&value[0][0], data, Rows * Columns * sizeof(T)); }
	Matrix(const vec<Rows, vec<Columns, T>>& data) : value(data) {}
	Matrix(T const (&data)[Rows * Columns])
	{
		memcpy((void*)&value[0][0], data, Rows * Columns * sizeof(T));
	}
	Matrix(Matrix<Rows, Columns, T> const& o) { memcpy((void*)&value[0][0], (void*)&o.value[0], Rows * Columns * sizeof(T)); }

	template <unsigned int ORows, unsigned int OColumns, typename OT>
	Matrix(Matrix<ORows, OColumns, OT> const& o)
	{
		int i, j;
		for(i = 0; i < Math::Min(ORows, Rows); i++)
		{
			for(j = 0; j < Math::Min(OColumns, Columns); j++)
			{
				value[i][j] = o[i][j];
			}
		}
		for(i; i < Rows; i++)
			for(j = 0; j < Columns; j++)
				if constexpr(Rows == Columns)
					value[i][j] = (i == j);
				else
					value[i][j] = 0;
	}

	protected:
	vec<Rows, vec<Columns, T>> value;

	template <unsigned int Rows, unsigned int Columns, typename T>
	friend std::ostream& operator<<(std::ostream&, const Matrix<Rows, Columns, T>&);
};
template <unsigned int Rows, unsigned int Columns, typename T = float>
std::ostream& operator<<(std::ostream& os, const Matrix<Rows, Columns, T>& o)
{
	os << "Matrix" << Rows << 'x' << Columns << typeid(T).name()[0] << '\n';

	for(int i = 0; i < Rows; i++)
	{
		os << '|';
		for(int j = 0; j < Columns - 1; j++) os << o.value[i][j] << ", ";
		os << o.value[i][Columns - 1] << '|' << '\n';
	}

	return os;
}

struct Matrix4x4 : public Matrix<4, 4>
{
	Matrix4x4(float a1 = 1, float a2 = 0, float a3 = 0, float a4 = 0, float b1 = 0, float b2 = 1, float b3 = 0, float b4 = 0, float c1 = 0, float c2 = 0, float c3 = 1, float c4 = 0, float d1 = 0, float d2 = 0, float d3 = 0, float d4 = 1)
		: Matrix({ a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4 })
	{
	}
	Matrix4x4(vec4f a, vec4f b, vec4f c, vec4f d)
		: Matrix(vec({ a, b, c, d }))
	{
	}
	template <unsigned int ORows, unsigned int OColumns, typename OT = float>
	Matrix4x4(const Matrix<ORows, OColumns, OT>& o) : Matrix(o) {}
	template <unsigned int ORows, unsigned int OColumns, typename OT = float>
	void operator =(const Matrix<ORows, OColumns, OT>& o) { this->Matrix::operator=(o); }

#pragma region TRANSFORMING
	vec3f TransformVector(vec4f a)
	{
		a.w = 1;
		return vec<3>(&(*this * Matrix(&a)));
	}
	vec3f TransformDir(vec4f a)
	{
		a.w = 0;
		return vec<3>(&(*this * Matrix(&a)));
	}

	void Scale(float x, float y, float z)
	{
		Matrix4x4 scaleMat
		{
			x,0,0,0,
			0,y,0,0,
			0,0,z,0,
			0,0,0,1
		};
		*this = scaleMat * *this;
	}
	void Scale(vec3f a) { Scale(a.x, a.y, a.z); }
	void Translate(float x, float y, float z)
	{
		Matrix4x4 posMat
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			x,y,z,1
		};
		*this = posMat * *this;
	}
	void Translate(vec3f a) { Translate(a.x, a.y, a.z); }
	void RotateX(float a)
	{
		if(a == 0)return;
		a = a * Math::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		Matrix4x4 rotMat(
			1, 0, 0, 0,
			0, Cos, Sin, 0,
			0, -Sin, Cos, 0,
			0, 0, 0, 1
		);
		*this = rotMat * *this;
	}
	void RotateY(float a)
	{
		if(a == 0)return;
		a = a * Math::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		Matrix4x4 rotMat(
			Cos, 0, -Sin, 0,
			0, 1, 0, 0,
			Sin, 0, Cos, 0,
			0, 0, 0, 1
		);
		*this = rotMat * *this;
	}
	void RotateZ(float a)
	{
		if(a == 0)return;
		a = a * Math::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		Matrix4x4 rotMat(
			Cos, Sin, 0, 0,
			-Sin, Cos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
		*this = rotMat * *this;
	}
	void Rotate(float x, float y, float z) { RotateZ(z);  RotateY(y); RotateX(x); }
	void Rotate(vec3f a) { Rotate(a.x, a.y, a.z); }

	void Transform(vec3f pos, vec3f scale, vec3f rot) { Rotate(rot); Scale(scale); Translate(pos); }
	void InverseTransform(vec3f pos, vec3f scale, vec3f rot) { Translate(-pos);  Rotate(-rot); Scale(vec3f(1, 1, 1) / scale); }
#pragma endregion
};
struct Matrix3x3 : public Matrix<3, 3>
{
	Matrix3x3(float a1 = 1, float a2 = 0, float a3 = 0, float b1 = 0, float b2 = 1, float b3 = 0, float c1 = 0, float c2 = 0, float c3 = 1)
		: Matrix({ a1,a2,a3,b1,b2,b3,c1,c2,c3 })
	{
	}
	Matrix3x3(vec3f a, vec3f b, vec3f c, vec3f d)
		: Matrix(vec({ a, b, c, d }))
	{
	}
	template <unsigned int ORows, unsigned int OColumns, typename OT = float>
	Matrix3x3(const Matrix<ORows, OColumns, OT>& o) : Matrix(o) {}
	template <unsigned int ORows, unsigned int OColumns, typename OT = float>
	void operator =(const Matrix<ORows, OColumns, OT>& o) { this->Matrix::operator=(o); }

#pragma region TRANSFORMING
	vec2f TransformVector(vec3f a)
	{
		a.z = 1;
		return vec<2>(&(*this * Matrix(&a)));
	}
	vec2f TransformDir(vec3f a)
	{
		a.z = 0;
		return vec<2>(&(*this * Matrix(&a)));
	}

	void Scale(float x, float y)
	{
		Matrix3x3 scaleMat
		{
			x,0,0,
			0,y,0,
			0,0,1
		};
		*this = scaleMat * *this;
	}
	void Scale(vec2f a) { Scale(a.x, a.y); }
	void Translate(float x, float y)
	{
		Matrix3x3 posMat
		{
			1,0,0,
			0,1,0,
			x,y,1
		};
		*this = posMat * *this;
	}
	void Translate(vec2f a) { Translate(a.x, a.y); }
	void Rotate(float a)
	{
		if(a == 0) return;
		a = a * Math::RAD;

		float Sin = sin(a);
		float Cos = cos(a);
		Matrix3x3 rotMat(
			Cos, Sin, 0,
			-Sin, Cos, 0,
			0, 0, 1
		);
		*this = rotMat * *this;
	}

	void Transform(vec2f pos, vec2f scale, float rot) { Scale(scale); Rotate(rot);  Translate(pos); }
	void InverseTransform(vec2f pos, vec2f scale, float rot) { Translate(-pos);  Rotate(-rot); Scale(vec2f(1, 1) / scale); }
#pragma endregion
};

namespace Matrices
{
	Matrix3x3 pivot(vec2f pivot, vec2f scale, float rotation)
	{
		Matrix3x3 mat = Matrix3x3();
		mat.Translate(pivot * -1);
		mat.Scale(scale);
		mat.Rotate(rotation);
		mat.Translate(pivot);
		return mat;
	}
	Matrix4x4 pivot(vec3f pivot, vec3f scale, vec3f rotation)
	{
		Matrix4x4 mat = Matrix4x4();
		mat.Translate(pivot * -1);
		mat.Rotate(scale);
		mat.Rotate(rotation);
		mat.Translate(pivot);
		return mat;
	}
	Matrix4x4 lookAt(vec3f position, vec3f target, vec3f up)
	{
		vec3f d = Vector::Normalize(position - target);
		vec3f r = Vector::Normalize(Vector::Cross(up, d));
		vec3f u = Vector::Cross(d, r);

		Matrix4x4 lookAt(
			r.x, u.x, d.x, 0,
			r.y, u.y, d.y, 0,
			r.z, u.z, d.z, 0,
			-Vector::Dot(r, position), -Vector::Dot(u, position), -Vector::Dot(d, position), 1
		);
		return lookAt;
	}
	Matrix4x4 Perspective(float ratio, float fov, float near, float far)
	{
		float s = 1 / (tan(fov / 2 * 3.14159265359f / 180.0f));
		return Matrix4x4(
			1 / s * ratio, 0, 0, 0,
			0, s, 0, 0,
			0, 0, -far / (far - near), -1,
			0, 0, -(far * near) / (far - near), 0
		);
	}
	Matrix4x4 Orthograpic(float left, float right, float bottom, float top, float near, float far)
	{
		return Matrix4x4(
			2 / (right - left), 0, 0, 0,
			0, -2 / (top - bottom), 0, 0,
			0, 0, 2 / (far - near), 0,
			-(right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near), 1
		);
	}
}