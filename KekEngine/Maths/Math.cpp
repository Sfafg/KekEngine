#include "Math.h"
#include <math.h>
#include <random>

const float Math::PI = 3.14159265359f;
const float Math::RAD = PI / 180.0f;
const float Math::DEGREES = 180.0f / PI;

float Math::Sin(float a)
{
	return sin(a);
};
float Math::ArcSin(float a)
{
	return asin(a);
};
float Math::Cos(float a)
{
	return cos(a);
};
float Math::ArcCos(float a)
{
	return acos(a);
};
float Math::Tan(float a)
{
	return tan(a);
};
float Math::Ceil(float a)
{
	return ceil(a);
}
float Math::Round(float a)
{
	return round(a);
}
float Math::Sqrt(float a)
{
	return sqrt(a);
}
std::default_random_engine generator;
float Math::NormalDist(float mean, float standardDeviation)
{
	std::normal_distribution<float> distribution(5.0, 2.0);

	return distribution(generator);
}
float Math::FastInverseRoot(float n)
{
	const float threehalfs = 1.5F;
	float y = n;

	long i = *(long*)&y;

	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;

	y = y * (threehalfs - ((n * 0.5F) * y * y));

	return y;
}