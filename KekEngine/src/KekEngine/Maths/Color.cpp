#include "Color.h"
namespace Kek
{
	rgb::rgb() {}
	rgb::rgb(float r, float g = 0, float b = 0) : col{ r,g,b } {}
	rgb::rgb(int v) : col(v) {}
	rgb::rgb(const std::string_view& v) : col(v) {}

	rgba::rgba() {}
	rgba::rgba(float r, float g = 0, float b = 0, float a = 0) : col{ r,g,b,a } {}
	rgba::rgba(int v) : col(v) {}
	rgba::rgba(const std::string_view& v) : col(v) {}

	rgb8i::rgb8i() {};
	rgb8i::rgb8i(unsigned char r, unsigned char g, unsigned char b) : col{ r,g,b } {}
	rgb8i::rgb8i(int v) : col(v) {}
	rgb8i::rgb8i(const std::string_view& v) : col(v) {}

	rgba8i::rgba8i() {};
	rgba8i::rgba8i(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : col{ r,g,b,a } {}
	rgba8i::rgba8i(int v) : col(v) {}
	rgba8i::rgba8i(const std::string_view& v) : col(v) {}
}