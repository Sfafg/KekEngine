#pragma once
#include <math.h>
#include <random>
namespace Math
{
	float PI = 3.14159265359f;
	float RAD = PI / 180.0f;

	template <typename T>
	T Abs(T a) { return abs(a); }
	template <typename T>
	T Sign(T a) { if(a < 0)return -1; if(a > 0) return 1; return 0; }
	float Ceil(float a) { return ceil(a); }
	float Round(float a) { return round(a); }

	template <typename T>
	T Min(T a, T b) { if(a > b)return b; return a; }
	template <typename T>
	T Max(T a, T b) { if(a < b)return b; return a; }
	template <typename T>
	T Clamp(T x, T min, T max) { return Max(Min(x, max), min); }
	template <typename T>
	T Lerp(T a, T b, float k) { return (1.0f - k) * a + b * k; }

	float Sqrt(float a) { return sqrt(a); }

	template <typename T>
	T Random(T min, T max) { return (T)rand() / RAND_MAX * (max - min) + min; }
	int Random(int min, int max) { return rand() % (max - min) + min; }

	namespace { std::default_random_engine generator; }
	float NormalDist(float mean, float standardDeviation)
	{
		std::normal_distribution<float> distribution(5.0, 2.0);

		return distribution(generator);
	}
	template <typename T>
	T Wrap(T x, T Min, T Max)
	{
		float mod = fmod(x - Min, Max - Min);
		if(mod < 0) return Max + mod;
		return mod + Min;
	}
	int Wrap(int x, int Min, int Max)
	{
		int mod = (x - Min) % (Max - Min);
		if(mod < 0) return Max + mod;
		return mod + Min;
	}

	float FastInverseRoot(float n)
	{
		const float threehalfs = 1.5F;
		float y = n;

		long i = *(long*)&y;

		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;

		y = y * (threehalfs - ((n * 0.5F) * y * y));

		return y;
	}
};