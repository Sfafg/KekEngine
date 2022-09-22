#include "Color.h"
namespace Kek
{
	colRGB::colRGB() {}
	colRGB::colRGB(float r, float g = 0, float b = 0) : col{ r,g,b } {}
	colRGB::colRGB(int v) : col(v) {}
	colRGB::colRGB(const std::string_view& v) : col(v) {}

	colRGBA::colRGBA() {}
	colRGBA::colRGBA(float r, float g = 0, float b = 0, float a = 0) : col{ r,g,b,a } {}
	colRGBA::colRGBA(int v) : col(v) {}
	colRGBA::colRGBA(const std::string_view& v) : col(v) {}

	colRGB8i::colRGB8i() {};
	colRGB8i::colRGB8i(unsigned char r, unsigned char g, unsigned char b) : col{ r,g,b } {}
	colRGB8i::colRGB8i(int v) : col(v) {}
	colRGB8i::colRGB8i(const std::string_view& v) : col(v) {}

	colRGBA8i::colRGBA8i() {};
	colRGBA8i::colRGBA8i(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : col{ r,g,b,a } {}
	colRGBA8i::colRGBA8i(int v) : col(v) {}
	colRGBA8i::colRGBA8i(const std::string_view& v) : col(v) {}
}