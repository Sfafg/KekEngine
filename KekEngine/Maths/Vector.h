#pragma once
#include <iostream>
#include "Math.h"

// Syntax trick to get x, y, z, w aliases for array values
template <unsigned int TSize, typename T> struct vec_union { vec_union() {}T value[TSize]; };
template <typename T> struct vec_union<2, T> { vec_union() {} union { T value[2]; struct { T x, y; }; }; };
template <typename T> struct vec_union<3, T> { vec_union() {} union { T value[3]; struct { T x, y, z; }; }; };
template <typename T> struct vec_union<4, T> { vec_union() {} union { T value[4]; struct { T x, y, z, w; }; }; };

template <unsigned int TSize, typename T = float>
struct vec : public vec_union<TSize, T>
{
	int Size() { return TSize; }
	float Mag() const
	{
		float mag = 0;
		for(const T& v : this->value) mag += v * v;
		return mag;
	}
	float Length() const { return Math::Sqrt(Mag()); }
	vec<TSize, T> Normalize() { float l = Length(); if(l != 0) *this /= l; return *this; }
	vec<TSize, T> Round() { for(T& v : this->value) v = Math::Round(v); return *this; }
	vec<TSize, T> Ceil() { for(T& v : this->value) v = Math::Ceil(v); return *this; }
	vec<TSize, T> Abs() { for(T& v : this->value) v = Math::Abs(v);  return *this; }
	vec<TSize, T> Sign() { for(T& v : this->value) v = Math::Sign(v); return *this; }
	vec<TSize, T> Clamp(vec<TSize, T> min, vec<TSize, T> max)
	{
		for(int i = 0; i < TSize; i++)
			operator[](i) = Math::Clamp(operator[](i), min[i], max[i]);
		return *this;
	}
	vec<TSize, T> Wrap(vec<TSize, T> min, vec<TSize, T> max)
	{
		for(int i = 0; i < TSize; i++)
			operator[](i) = Math::Wrap(operator[](i), min[i], max[i]);
		return *this;
	}

	void operator += (const T& v) { for(T& val : this->value) val += v; }
	void operator -= (const T& v) { for(T& val : this->value) val -= v; }
	void operator *= (const T& v) { for(T& val : this->value) val *= v; }
	void operator /= (const T& v) { for(T& val : this->value) val /= v; }

	void operator += (const vec<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) += v[i]; }
	void operator -= (const vec<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) -= v[i]; }
	void operator *= (const vec<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) *= v[i]; }
	void operator /= (const vec<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) /= v[i]; }

	vec<TSize, T> operator + (const T& v) const { vec<TSize, T> vec(*this); vec += v; return vec; }
	vec<TSize, T> operator - (const T& v) const { vec<TSize, T> vec(*this); vec -= v; return vec; }
	vec<TSize, T> operator * (const T& v) const { vec<TSize, T> vec(*this); vec *= v; return vec; }
	vec<TSize, T> operator / (const T& v) const { vec<TSize, T> vec(*this); vec /= v; return vec; }

	vec<TSize, T> operator + (const vec<TSize, T>& v) const { vec<TSize, T> vec(*this); vec += v; return vec; }
	vec<TSize, T> operator - (const vec<TSize, T>& v) const { vec<TSize, T> vec(*this); vec -= v; return vec; }
	vec<TSize, T> operator * (const vec<TSize, T>& v) const { vec<TSize, T> vec(*this); vec *= v; return vec; }
	vec<TSize, T> operator / (const vec<TSize, T>& v) const { vec<TSize, T> vec(*this); vec /= v; return vec; }

	vec<TSize, T> operator - () const { vec<TSize, T> vec(*this);  for(T& val : vec.value) val = -val; return vec; }

	bool operator ==(const T& v) const { for(T& val : this->value) if(val != v) return false; return true; }
	bool operator !=(const T& v) const { for(T& val : this->value) if(val == v) return false; return true; }
	bool operator < (const T& v) const { for(T& val : this->value) if(val >= v) return false; return true; }
	bool operator > (const T& v) const { for(T& val : this->value) if(val <= v) return false; return true; }
	bool operator <=(const T& v) const { for(T& val : this->value) if(val > v) return false; return true; }
	bool operator >=(const T& v) const { for(T& val : this->value) if(val < v) return false; return true; }

	bool operator == (const vec<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) != v[i]) return false; return true; }
	bool operator != (const vec<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) == v[i]) return false; return true; }
	bool operator <  (const vec<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) >= v[i]) return false; return true; }
	bool operator >  (const vec<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) <= v[i]) return false; return true; }
	bool operator <= (const vec<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) > v[i]) return false; return true; }
	bool operator >= (const vec<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) < v[i]) return false; return true; }

	T& operator[](unsigned int index) { return this->value[index]; }
	const T& operator[](unsigned int index) const { return this->value[index]; }
	operator T* () { return &(this->value[0]); }
	operator const T* () const { return &(this->value[0]); }

	vec() {}
	vec(T data) { for(T& v : this->value) v = data; }
	vec(T const (&data)[TSize]) { memcpy(this->value, data, sizeof(T) * TSize); }
	vec(std::initializer_list<T> data) { memcpy(this->value, data.begin(), sizeof(T) * TSize); }

	template <unsigned int OTSize, typename OT>
	void operator =(const vec<OTSize, OT>& o)
	{
		constexpr int min = OTSize < TSize ? OTSize : TSize;
		for(int i = 0; i < min; i++) (*this)[i] = o[i];
	}
	template <unsigned int OTSize, typename OT>
	vec(const vec<OTSize, OT>& o)
	{
		constexpr int min = OTSize < TSize ? OTSize : TSize;
		int i = 0;
		for(i; i < min; i++)(*this)[i] = o[i];

		constexpr int max = OTSize > TSize ? OTSize : TSize;
		for(i; i < max; i++)(*this)[i] = 0;
	}
};
template <unsigned int TSize, typename T>
std::ostream& operator<<(std::ostream& os, const vec<TSize, T>& o)
{
	os << "[{LCyan}]<vec" << TSize << typeid(T).name()[0] << ">[{LYellow}]<(";

	for(int i = 0; i < TSize - 1; i++) os << o.value[i] << ", ";

	os << o.value[TSize - 1] << ")> ";

	return os;
}

namespace Vector
{
	template <unsigned int TSize, typename T> vec<TSize, T> Length(vec<TSize, T> v) { return v.Length(); }
	template <unsigned int TSize, typename T> vec<TSize, T> Distance(vec<TSize, T> v, vec<TSize, T> v1) { return Length(v - v1); }

	template <unsigned int TSize, typename T> vec<TSize, T> Normalize(vec<TSize, T> v) { return v.Normalize(); }
	template <unsigned int TSize, typename T> vec<TSize, T> Round(vec<TSize, T> v) { return v.Round(); }
	template <unsigned int TSize, typename T> vec<TSize, T> Ceil(vec<TSize, T> v) { return v.Ceil(); }
	template <unsigned int TSize, typename T> vec<TSize, T> Abs(vec<TSize, T> v) { return v.Abs(); }
	template <unsigned int TSize, typename T> vec<TSize, T> Sign(vec<TSize, T> v) { return v.Sign(); }
	template <unsigned int TSize, typename T> vec<TSize, T> Clamp(vec<TSize, T> v, const vec<TSize, T>& min, const vec<TSize, T>& max) { return v.Clamp(min, max); }
	template <unsigned int TSize, typename T> vec<TSize, T> Wrap(vec<TSize, T> v, const vec<TSize, T>& min, const vec<TSize, T>& max) { return v.Wrap(min, max); }
	template <unsigned int TSize, typename T> vec<TSize, T> Lerp(const vec<TSize, T>& a, const vec<TSize, T>& b, float k) { return Math::Lerp(a, b, k); }

	template <unsigned int TSize, typename T>
	float Dot(const vec<TSize, T>& a, const vec<TSize, T>& b)
	{
		float sum = 0;
		for(int i = 0; i < TSize; i++) sum += a[i] * b[i];
		return sum;
	}
	template <typename T>
	vec<2, T> Cross(const vec<2, T>& a)
	{
		return vec<2, T>({ -a[1], a[0] });
	}
	template <typename T>
	vec<3, T> Cross(const vec<3, T>& a, const vec<3, T>& b)
	{
		return vec<3, T>{a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
	}
	template <typename T>
	vec<3, T> Reflect(const vec<3, T>& k, vec<3, T> nor)
	{
		float mag = dot(k, nor) * 2;
		nor *= mag;
		nor -= k;
		return nor;
	}

	template <unsigned int TSize, typename T>
	float Angle(const vec<TSize, T>& a, const vec<TSize, T>& b)
	{
		return acos(Dot(a, b) / a.Length() * b.Length());
	}
	template <typename T>
	vec<2, T> Polar(float a, float r = 1) { return vec<2, T>({ Math::Cos(a) * r, Math::Sin(a) * r }); }
	template <typename T>
	vec<3, T> Spherical(float a, float b, float r = 1)
	{
		float c = Math::Cos(b * 0.01745329251f) * r;
		return vec<3, T>({ Math::Sin(a * 0.01745329251f) * c, Math::Sin(b * 0.01745329251f) * r, Math::Cos(a * 0.01745329251f) * c });
	}

	template <unsigned int TSize, typename T>
	vec<TSize, T> Random(float min = 0, float max = 1)
	{
		vec<TSize, T> r;
		for(int i = 0; i < TSize; i++) r[i] = Math::Random(min, max);
		return r;
	}
	template <unsigned int TSize, typename T>
	vec<TSize, T> Random(const vec<TSize, T>& min, const vec<TSize, T>& max)
	{
		vec<TSize, T> r;
		for(int i = 0; i < TSize; i++) r[i] = Math::Random(min[i], max[i]);
		return r;
	}
	template <unsigned int TSize, typename T>
	vec<TSize, T> Unit()
	{
		return Vector::Random<TSize, T>(-1.0f, 1.0f).Normalize();
	}
};

struct vec2f : public vec<2>
{
	vec2f();
	vec2f(float x, float y = 0);
	template <unsigned int TSize, typename T>
	vec2f(const vec<TSize, T>& v) : vec(v) {}
};
struct vec3f : public vec<3>
{
	vec3f();
	vec3f(float x, float y = 0, float z = 0);
	template <unsigned int TSize, typename T>
	vec3f(const vec<TSize, T>& v) : vec(v) {}
};
struct vec4f : public vec<4>
{
	vec4f();
	vec4f(float x, float y = 0, float z = 0, float w = 0);
	template <unsigned int TSize, typename T>
	vec4f(const vec<TSize, T>& v) : vec(v) {}
};