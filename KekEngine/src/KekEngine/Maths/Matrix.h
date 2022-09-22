#pragma once
#include <iosfwd>
#include "Vector.h"

template <unsigned int TRows, unsigned int TColumns = 1, typename T = float>
struct mat
{
	int Size() const { return TRows * TColumns; }
	int Rows()  const { return TRows; }
	int Columns()  const { return TColumns; }
	void Transpose()
	{
		for(int i = 0; i < TRows; i++)
		{
			for(int j = i + 1; j < TColumns; j++)
			{
				T temp = value[i][j];
				value[i][j] = value[j][i];
				value[j][i] = temp;
			}
		}
	}

	void operator *= (const mat<TRows, TColumns, T>& m)
	{
		mat<TRows, TColumns, T> result;

		for(int i = 0; i < TRows; i++)
		{
			for(int j = 0; j < TColumns; j++)
			{
				result[i][j] = 0;
				for(int k = 0; k < TColumns; k++)
				{
					result[i][j] += (*this)[i][k] * m[k][j];
				}
			}
		}

		*this = result;
	}
	template <unsigned int OTColumns>
	mat<TRows, OTColumns, T> operator *(const mat<TRows, OTColumns, T>& m) const
	{
		mat<TRows, OTColumns, T> result;

		for(int i = 0; i < TRows; i++)
		{
			for(int j = 0; j < OTColumns; j++)
			{
				result[i][j] = 0;
				for(int k = 0; k < TColumns; k++)
				{
					result[i][j] += (*this)[i][k] * m[k][j];
				}
			}
		}

		return result;
	}

	auto& operator[](int index) { return value[index]; }
	const auto& operator[](int index) const { return value[index]; }
	operator T* () { return &this->value[0][0]; }
	operator const T* () const { return &this->value[0][0]; }

	mat()
	{
		if constexpr(TRows == TColumns)
		{
			for(int i = 0; i < TRows; i++)
				for(int j = 0; j < TColumns; j++)
					value[i][j] = (i == j);
		}
	}
	mat(T a)
	{
		for(int i = 0; i < TRows; i++)
			for(int j = 0; j < TColumns; j++)
				value[i][j] = a;
	}
	mat(const vec<TRows>& data) { memcpy(value, data, Size() * sizeof(T)); }
	mat(const vec<TRows, vec<TColumns, T>>& data) { memcpy(value, data, Size() * sizeof(T)); }
	mat(T const (&data)[TRows * TColumns])
	{
		memcpy(value, data, Size() * sizeof(T));
	}
	mat(std::initializer_list<T> data)
	{
		memcpy(value, data.begin(), TRows * TColumns * sizeof(T));
	}

	template <unsigned int OTRows, unsigned int OTColumns, typename OT>
	mat(mat<OTRows, OTColumns, OT> const& o)
	{
		int i, j;
		for(i = 0; i < Maths::Min(OTRows, TRows); i++)
		{
			for(j = 0; j < Maths::Min(OTColumns, TColumns); j++)
			{
				value[i][j] = o[i][j];
			}
		}
		for(i; i < TRows; i++)
			for(j = 0; j < TColumns; j++)
				if constexpr(TRows == TColumns)
					value[i][j] = (i == j);
				else
					value[i][j] = 0;
	}

	operator vec<TRows* TColumns>() { vec<TRows* TColumns> a; memcpy(&a, value, Size() * sizeof(T)); return a; }

	protected:
	T value[TRows][TColumns];

	template <unsigned int TRows, unsigned int TColumns, typename T>
	friend std::ostream& operator<<(std::ostream&, const mat<TRows, TColumns, T>&);
};
template <unsigned int TRows, unsigned int TColumns, typename T = float>
std::ostream& operator<<(std::ostream& os, const mat<TRows, TColumns, T>& m)
{
	os << "[{LCyan}]<mat" << TRows << 'x' << TColumns << typeid(T).name()[0] << ">\n";
	os << "[{LYellow}]<";
	for(int i = 0; i < TRows - 1; i++)
	{
		os << '|';
		for(int j = 0; j < TColumns - 1; j++) os << m[i][j] << ", ";
		os << m[i][TColumns - 1] << '|' << '\n';
	}
	os << '|';
	for(int j = 0; j < TColumns - 1; j++) os << m[TRows - 1][j] << ", ";
	os << m[TRows - 1][TColumns - 1] << '|';
	os << '>';
	return os;
}

namespace Matrix
{
	template <unsigned int TRows, unsigned int TColumns = 1, typename T = float>
	mat<TRows, TColumns, T> Transpose(mat<TRows, TColumns, T> mat)
	{
		mat.Transpose();
		return mat;
	}
}

struct mat4x4 : public mat<4, 4>
{
	mat4x4();
	mat4x4(
		float a1, float a2, float a3, float a4,
		float b1, float b2, float b3, float b4,
		float c1, float c2, float c3, float c4,
		float d1, float d2, float d3, float d4);
	mat4x4(vec4f a, vec4f b, vec4f c, vec4f d);

	template <unsigned int OTRows, unsigned int OTColumns, typename OT>
	mat4x4(const mat<OTRows, OTColumns, OT>& o) : mat(o) {}
	template <unsigned int OTRows, unsigned int OTColumns, typename OT>
	void operator =(const mat<OTRows, OTColumns, OT>& o) { this->mat::operator=(o); }

	vec3f TransformVector(vec4f a) const;
	vec3f TransformDir(vec4f a) const;

	void Scale(float x, float y, float z);
	void Scale(const vec3f& a);
	void Translate(float x, float y, float z);
	void Translate(const vec3f& a);
	void RotateX(float a);
	void RotateY(float a);
	void RotateZ(float a);
	void Rotate(float x, float y, float z);
	void Rotate(const vec3f& a);

	void Transform(const vec3f& pos, const vec3f& scale, const vec3f& rot);
	void InverseTransform(const vec3f& pos, const vec3f& scale, const vec3f& rot);
};
struct mat3x3 : public mat<3, 3>
{
	mat3x3();
	mat3x3(
		float a1, float a2, float a3,
		float b1, float b2, float b3,
		float c1, float c2, float c3);
	mat3x3(vec3f a, vec3f b, vec3f c, vec3f d);

	template <unsigned int OTRows, unsigned int OTColumns, typename OT = float>
	mat3x3(const mat<OTRows, OTColumns, OT>& o) : mat(o) {}
	template <unsigned int OTRows, unsigned int OTColumns, typename OT = float>
	void operator =(const mat<OTRows, OTColumns, OT>& o) { this->mat::operator=(o); }

	vec2f TransformVector(vec3f a) const;
	vec2f TransformDir(vec3f a) const;

	void Scale(float x, float y);
	void Scale(const vec2f& a);
	void Translate(float x, float y);
	void Translate(const vec2f& a);
	void Rotate(float a);

	void Transform(const vec2f& pos, const vec2f& scale, float rot);
	void InverseTransform(const vec2f& pos, const vec2f& scale, float rot);
};

namespace Matrix
{
	mat3x3 Pivot(vec2f pivot, vec2f scale, float rotation);
	mat4x4 Pivot(vec3f pivot, vec3f scale, vec3f rotation);
	mat4x4 LookAt(vec3f position, vec3f target, vec3f up);
	mat4x4 Perspective(float ratio, float fov, float near, float far);
	mat4x4 Orthograpic(float left, float right, float bottom, float top, float near, float far);
}