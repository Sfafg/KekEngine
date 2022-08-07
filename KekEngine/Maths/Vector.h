#pragma once
#include <iostream>
#include "Math.h"

namespace
{
	template <int size, typename T>
	struct vec_union
	{
		public:
		vec_union() {};
		vec_union(T* data) { memcpy((void*)&value[0], data, size * sizeof(T)); }
		vec_union(T const (&data)[size]) { memcpy((void*)&value[0], data, size * sizeof(T)); }
		T value[size];
	};

	template <typename T>
	struct vec_union<2, T>
	{
		public:
		vec_union() {};
		vec_union(T* data) { memcpy((void*)&value[0], data, 2 * sizeof(T)); }
		vec_union(T const (&data)[2]) { memcpy((void*)&value[0], data, 2 * sizeof(T)); }
		union { T value[2]; struct { T x, y; }; };
	};
	template <typename T>
	struct vec_union<3, T>
	{
		public:
		vec_union() {};
		vec_union(T* data) { memcpy((void*)&value[0], data, 3 * sizeof(T)); }
		vec_union(T const (&data)[3]) { memcpy((void*)&value[0], data, 3 * sizeof(T)); }
		union { T value[3]; struct { T x, y, z; }; };
	};
	template <typename T>
	struct vec_union<4, T>
	{
		public:
		vec_union() {};
		vec_union(T* data) { memcpy((void*)&value[0], data, 4 * sizeof(T)); }
		vec_union(T const (&data)[4]) { memcpy((void*)&value[0], data, 4 * sizeof(T)); }
		union { T value[4]; struct { T x, y, z, w; }; };
	};
}

template <unsigned int Size, typename T = float>
struct vec : public vec_union<Size, T>
{
	float Mag()
	{
		int total = 0;
		for(T& v : this->value) total += v * v;

		return total;
	}
	float Lenght()
	{
		return Math::Sqrt(Mag());
	}
	void Normalize()
	{
		float l = Lenght();
		if(l == 0)return;

		l = 1.0f / l;
		for(T& v : this->value) v *= l;
	}

	void Wrap(vec<Size, T> min, vec<Size, T> max)
	{
		for(int i = 0; i < size; i++)
			this->value[i] = Math::Clamp(this->value[i], min[i], max[i]);
	}
	void Clamp(vec<Size, T> min, vec<Size, T> max)
	{
		for(int i = 0; i < size; i++)
			this->value[i] = Math::Clamp(this->value[i], min[i], max[i]);
	}

	int size() { return sizeof(this->value) / sizeof(this->value[0]); }
#pragma region OPERATORS
	void operator += (vec<Size, T> const& o) { for(int i = 0; i < Size; i++) this->value[i] += o[i]; }
	void operator -= (vec<Size, T> const& o) { for(int i = 0; i < Size; i++) this->value[i] -= o[i]; }
	void operator *= (vec<Size, T> const& o) { for(int i = 0; i < Size; i++) this->value[i] *= o[i]; }
	void operator /= (vec<Size, T> const& o) { for(int i = 0; i < Size; i++) this->value[i] /= o[i]; }
	void operator  = (vec<Size, T> const& o) { for(int i = 0; i < Size; i++) this->value[i] = o[i]; }

	void operator += (T const& o) { for(T& v : this->value) v += o; }
	void operator -= (T const& o) { for(T& v : this->value) v -= o; }
	void operator *= (T const& o) { for(T& v : this->value) v *= o; }
	void operator /= (T const& o) { for(T& v : this->value) v /= o; }
	void operator  = (T const& o) { for(T& v : this->value) v = o; }

	vec<Size, T> operator + (vec<Size, T> const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] + o[i]; return temp; }
	vec<Size, T> operator - (vec<Size, T> const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] - o[i]; return temp; }
	vec<Size, T> operator * (vec<Size, T> const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] * o[i]; return temp; }
	vec<Size, T> operator / (vec<Size, T> const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] / o[i]; return temp; }

	vec<Size, T> operator + (T const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] + o; return temp; }
	vec<Size, T> operator - (T const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] - o; return temp; }
	vec<Size, T> operator * (T const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] * o; return temp; }
	vec<Size, T> operator / (T const& o) { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = this->value[i] / o; return temp; }

	vec<Size, T> operator - () { vec<Size, T> temp; for(int i = 0; i < Size; i++) temp[i] = -this->value[i]; return temp; }

	bool operator < (vec<Size, T> const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] < o[i]; if(!is)break; } return is; }
	bool operator > (vec<Size, T> const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] > o[i]; if(!is)break; } return is; }
	bool operator <= (vec<Size, T> const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] <= o[i]; if(!is)break; } return is; }
	bool operator >= (vec<Size, T> const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] >= o[i]; if(!is)break; } return is; }
	bool operator == (vec<Size, T> const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] == o[i]; if(!is)break; } return is; }
	bool operator != (vec<Size, T> const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] != o[i]; if(!is)break; } return is; }

	bool operator < (T const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] < o; if(!is)break; } return is; }
	bool operator > (T const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] > o; if(!is)break; } return is; }
	bool operator <= (T const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] <= o; if(!is)break; } return is; }
	bool operator >= (T const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] >= o; if(!is)break; } return is; }
	bool operator == (T const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] == o; if(!is)break; } return is; }
	bool operator != (T const& o) { bool is = false; for(int i = 0; i < Size; i++){ is = this->value[i] != o; if(!is)break; } return is; }

	T& operator[](int index) { return this->value[index]; }
	const T& operator[](int index) const { return this->value[index]; }

	T* operator &() { return &(this->value[0]); }
	explicit operator T* () { return &(this->value[0]); }
	explicit operator const T* () { return &(this->value[0]); }
#pragma endregion

	vec(T a = 0.f)
	{
		for(int i = 0; i < Size; i++) this->value[i] = a;
	}
	vec(T* data) : vec_union<Size, T>(data) {}
	vec(T const (&data)[Size]) : vec_union<Size, T>(data) {}

	template <unsigned int Osize, typename OT>
	vec(const vec<Osize, OT>& o)
	{
		int i = 0;
		for(i; i < Math::Min<unsigned int>(Size, Osize); i++)
			this->value[i] = o[i];
		for(i; i < Size; i++) this->value[i] = 0;
	}

	template <unsigned int Size, typename T>
	friend std::ostream& operator<<(std::ostream&, const vec<Size, T>&);
};

template <unsigned int size, typename T>
std::ostream& operator<<(std::ostream& os, const vec<size, T>& o)
{
	os << "vec" << size << typeid(T).name()[0] << "(";

	for(int i = 0; i < size - 1; i++)
		os << o.value[i] << ", ";

	os << o.value[size - 1] << ") ";

	return os;
}

struct vec2f : public vec<2, float>
{
	vec2f(float x = 0, float y = 0) : vec({ x, y }) {}

	template <unsigned int Osize, typename OT>
	vec2f(const vec<Osize, OT>& o) { for(int i = 0; i < Math::Min<unsigned int>(2, Osize); i++) this->value[i] = o[i]; }
};
struct vec2i : public vec<2, int>
{
	vec2i(int x = 0, int y = 0) : vec({ x, y }) {}

	template <unsigned int Osize, typename OT>
	vec2i(const vec<Osize, OT>& o) { for(int i = 0; i < Math::Min<unsigned int>(2, Osize); i++) this->value[i] = o[i]; }
};
struct vec3f : public vec<3, float>
{
	vec3f(float x = 0, float y = 0, float z = 0) : vec({ x, y, z }) {}

	template <unsigned int Osize, typename OT>
	vec3f(const vec<Osize, OT>& o) { for(int i = 0; i < Math::Min<unsigned int>(3, Osize); i++) this->value[i] = o[i]; }
};
struct vec4f : public vec<4, float>
{
	vec4f(float x = 0, float y = 0, float z = 0, float w = 0) : vec({ x,y,z ,w }) {}
	vec4f(vec3f a, float w_ = 0) : vec({ a.x, a.y, a.z, w }) {}

	template <unsigned int Osize, typename OT>
	vec4f(const vec<Osize, OT>& o) { for(int i = 0; i < Math::Min<unsigned int>(4, Osize); i++) this->value[i] = o[i]; }
};

namespace Vector
{
	template <unsigned int size>
	vec<size, int> Round(vec<size, float> const& a)
	{
		int temp[size];
		for(int i = 0; i < size; i++) temp[i] = Math::Round(a[i]);

		return vec<size, int>(temp);
	}
	template <unsigned int size>
	vec<size, int> Ceil(vec<size, float> const& a)
	{
		int temp[size];
		for(int i = 0; i < size; i++) temp[i] = Math::Ceil(a[i]);

		return vec<size, int>(temp);
	}
	template <unsigned int size, typename T>
	vec<size, T> Abs(vec<size, T> const& a)
	{
		T temp[size];
		for(int i = 0; i < size; i++) temp[i] = Math::Abs(a[i]);

		return vec<size, T>(temp);
	}

	template <unsigned int size, typename T>
	vec<size, T> Wrap(vec<size, T> a, vec<size, T> const& min, vec<size, T> const& max)
	{
		return a.Wrap(min, max);
	}
	template <unsigned int size, typename T>
	vec<size, T> Clamp(vec<size, T> a, vec<size, T> const& min, vec<size, T> const& max)
	{
		return a.Clamp(min, max);
	}

	template <unsigned int size, typename T>
	float Mag(vec<size, T> const& a) { return a.Mag(); }
	template <unsigned int size, typename T>
	float Length(vec<size, T> const& a) { return a.Length(); }
	template <unsigned int size, typename T>
	float Dot(vec<size, T> const& a, vec<size, T> const& b)
	{
		int sum = 0;
		for(int i = 0; i < size; i++) sum += a[i] * b[i];

		return sum;
	}
	template <unsigned int size, typename T>
	vec<size, T> Normalize(vec<size, T> a)
	{
		a.Normalize();
		return a;
	}
	template <typename T>
	vec<2, T> Cross(vec<2, T> const& a)
	{
		return vec<2, T>({ -a[1], a[0] });
	}
	template <typename T>
	vec<3, T> Cross(vec<3, T> const& a, vec<3, T> b)
	{
		return vec<3, T>({ a[1] * b[2] - a[2] * b[1],a[2] * b[0] - a[0] * b[2],a[0] * b[1] - a[1] * b[0] });
	}
	template <typename T>
	vec<3, T> Reflect(vec<3, T> const& k, vec<3, T> nor)
	{
		float mag = dot(k, nor) * 2;
		nor *= mag;
		nor -= k;

		return nor;
	}

	template <typename T>
	float AngleVec(vec<2, T> const& a, vec<2, T> const& b)
	{
		return acos(Dot(a, b) / (a.Lenght() * b.Lengh())) * Math::Sign(Dot(Cross(a), b));
	}
	template <typename T>
	vec<2, T> Polar(float a, float r = 1)
	{
		return vec<2, T>({ cos(a) * r, sin(a) * r });
	}
	template <typename T>
	vec<3, T> SphCoord(float azimuth, float theta, float r = 1)
	{
		float c = cos(theta * 0.01745329251f) * r;
		return vec<3, T>({ sin(azimuth * 0.01745329251f) * c, sin(theta * 0.01745329251f) * r, cos(azimuth * 0.01745329251f) * c });
	}

	template <int size, typename T>
	vec<size, T> RVec(vec<size, T> const& min, vec<size, T> const& max)
	{
		T temp[size];
		for(int i = 0; i < size; i++) temp[i] = random(min[i], max[i]);

		return vec<size, T>(temp);
	}
	template <int size, typename T>
	vec<size, T> RVec(float min, float max)
	{
		T temp[size];
		for(int i = 0; i < size; i++) temp[i] = Math::Random(min, max);

		return vec<size, T>(temp);
	}
	template <int size, typename T>
	vec<size, T> UVec()
	{
		T temp[size];
		for(int i = 0; i < size; i++) temp[i] = Math::NormalDist(0, 1);

		return Normalize(vec<size, T>(temp));
	}
	template <int size, typename T>
	vec<size, T> SVec(float radius)
	{
		return UVec<size, T>() * Math::Random(0, radius);
	}
}