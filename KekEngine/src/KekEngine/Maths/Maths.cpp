#include "Maths.h"
#include <math.h>
#include <random>
namespace Kek
{
	const float Maths::PI = 3.14159265359f;
	const float Maths::RAD = PI / 180.0f;
	const float Maths::DEGREES = 180.0f / PI;
	const int Maths::MAX_RAND = RAND_MAX;

	int Maths::Rand()
	{
		return rand();
	}

	float Maths::Sin(float a)
	{
		return sin(a);
	};
	float Maths::ArcSin(float a)
	{
		return asin(a);
	};
	float Maths::Cos(float a)
	{
		return cos(a);
	};
	float Maths::ArcCos(float a)
	{
		return acos(a);
	};
	float Maths::Tan(float a)
	{
		return tan(a);
	};
	float Maths::Ceil(float a)
	{
		return ceil(a);
	}
	float Maths::Round(float a)
	{
		return round(a);
	}
	float Maths::Pow(float a, float b)
	{
		return pow(a, b);
	}
	float Maths::Sqrt(float a)
	{
		return sqrt(a);
	}

	std::default_random_engine generator;
	float Maths::NormalDist(float mean, float standardDeviation)
	{
		std::normal_distribution<float> distribution(5.0, 2.0);

		return distribution(generator);
	}
	float Maths::FastInverseRoot(float n)
	{
		const float threehalfs = 1.5F;
		float y = n;

		long i = *(long*)&y;

		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;

		y = y * (threehalfs - ((n * 0.5F) * y * y));

		return y;
	}
}