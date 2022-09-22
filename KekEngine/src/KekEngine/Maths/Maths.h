#pragma once
namespace Kek
{
	namespace Maths
	{
		extern const float PI;
		extern const float RAD;
		extern const float DEGREES;
		extern const int MAX_RAND;

		int Rand();

		template <typename T> T Abs(T a) { return abs(a); }
		template <typename T> T Sign(T a) { if(a < 0)return -1; if(a > 0) return 1; return 0; }
		template <typename T> T Min(T a, T b) { if(a > b)return b; return a; }
		template <typename T> T Max(T a, T b) { if(a < b)return b; return a; }
		template <typename T> T Clamp(T x, T min, T max) { return Max(Min(x, max), min); }
		template <typename T> T Lerp(T a, T b, float k) { return a * (1.0f - k) + b * k; }
		template <typename T> T Wrap(T x, T Min, T Max) { float mod = fmod(x - Min, Max - Min); if(mod < 0) return Max + mod; return mod + Min; }
		template <typename T> T Random(T min, T max) { return (T)Rand() / MAX_RAND * (max - min) + min; }

		float Sin(float a);
		float ArcSin(float a);
		float Cos(float a);
		float ArcCos(float a);
		float Tan(float a);
		float Ceil(float a);
		float Round(float a);
		float Sqrt(float a);
		float NormalDist(float mean, float standardDeviation);
		float FastInverseRoot(float n);
	};
}