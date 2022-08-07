#pragma once
#include "Vectors.h"
#include <sstream>

class ColorRGB
{
	public:
	float r, g, b;

	ColorRGB(float r_ = 0, float g_ = 0, float b_ = 0) :
		r(r_), g(g_), b(b_)
	{
	}
	ColorRGB(vec3f col) :
		r(col.x), g(col.y), b(col.z)
	{
	}
	ColorRGB(int v)
	{
		b = v % 256; v /= 256;
		g = v % 256; v /= 256;
		constexpr float oneOver255 = 1.f / 255.f;
		r = float(v) * oneOver255;
		g = float(g) * oneOver255;
		b = float(b) * oneOver255;
	}
	ColorRGB(const std::string& c)
	{
		std::stringstream ss;
		ss << std::hex << c;
		int v;
		ss >> v;
		int b = v % 256; v /= 256;
		int g = v % 256; v /= 256;
		ColorRGB(v, g, b);
	}

	void setHSV(const vec3f& t)
	{
		const float piOver3 = 1.0471975512;
		float c = t.z * t.y; // 1
		float x = (1 - abs(fmod(t.x / piOver3, 2) - 1)) * c; // 1
		float m = t.z - c;	// 0
		int p = (int)floor(t.x / piOver3) % 6;	// 0

		switch(p)
		{
		case 0: r = c + m; g = x + m; b = m; break;
		case 1: r = x + m; g = c + m; b = m; break;
		case 2: r = m; g = c + m; b = x + m; break;
		case 3: r = m; g = x + m; b = c + m; break;
		case 4: r = x + m; g = m; b = c + m; break;
		default: r = c + m; g = m; b = x + m; break;
		}
	}

	void operator=(const ColorRGB& o) { r = o.r; g = o.g; b = o.b; }
	void operator+=(const ColorRGB& o) { r += o.r; g += o.g; b += o.b; }
	void operator-=(const ColorRGB& o) { r -= o.r; g -= o.g; b -= o.b; }
	void operator*=(const ColorRGB& o) { r *= o.r; g *= o.g; b *= o.b; }
	void operator/=(const ColorRGB& o) { r /= o.r; g /= o.g; b /= o.b; }

	ColorRGB operator+(const ColorRGB& o) { ColorRGB p = *this; p += o; return p; }
	ColorRGB operator-(const ColorRGB& o) { ColorRGB p = *this; p -= o; return p; }
	ColorRGB operator*(const ColorRGB& o) { ColorRGB p = *this; p *= o; return p; }
	ColorRGB operator/(const ColorRGB& o) { ColorRGB p = *this; p /= o; return p; }

	void operator=(const vec3f& o) { *this = o; }
	void operator+=(const vec3f& o) { *this += o; }
	void operator-=(const vec3f& o) { *this -= o; }
	void operator*=(const vec3f& o) { *this *= o; }
	void operator/=(const vec3f& o) { *this /= o; }

	vec3f operator+(const vec3f& o) { return *this + o; }
	vec3f operator-(const vec3f& o) { return *this + o; }
	vec3f operator*(const vec3f& o) { return *this + o; }
	vec3f operator/(const vec3f& o) { return *this + o; }

	ColorRGB operator - () { return ColorRGB(-r, -g, -b); }

	bool operator < (ColorRGB const& o) { return r < o.r&& g < o.g&& b < o.b; }
	bool operator > (ColorRGB const& o) { return r > o.r && g > o.g && b > o.b; }
	bool operator <= (ColorRGB const& o) { return r <= o.r && g <= o.g && b <= o.b; }
	bool operator >= (ColorRGB const& o) { return r >= o.r && g >= o.g && b >= o.b; }
	bool operator == (ColorRGB const& o) { return r == o.r && g == o.g && b == o.b; }
	bool operator != (ColorRGB const& o) { return r != o.r && g != o.g && b != o.b; }

	operator vec3f() { return vec3f(r, g, b); }
	/*vec3f getHSV()
	{
		const float piOver3 = 1.0471975512;
		float cmin = min(min(r, g), b);
		int t;
		if (r > g) {
		}
	}*/
};

std::ostream& operator<<(std::ostream& os, const ColorRGB& o) { os << "ColorRGB(" << o.r << "," << o.g << "," << o.b << ")"; return os; }

class ColorRGBA
{
	public:
	float r, g, b, a;

	ColorRGBA(float r_ = 0, float g_ = 0, float b_ = 0, float a_ = 1) :
		r(r_), g(g_), b(b_), a(a_)
	{
	}
	ColorRGBA(vec3f col) :
		r(col.x), g(col.y), b(col.z), a(1)
	{
	}
	ColorRGBA(vec4f col) :
		r(col.x), g(col.y), b(col.z), a(col.w)
	{
	}

	ColorRGBA(int v)
	{
		b = v % 256; v /= 256;
		g = v % 256; v /= 256;
		constexpr float oneOver255 = 1.f / 255.f;
		r = float(v) * oneOver255;
		g = float(g) * oneOver255;
		b = float(b) * oneOver255;
		a = 1.0f;
	}
	ColorRGBA(const std::string& c)
	{
		std::stringstream ss;
		ss << std::hex << c;
		int v;
		ss >> v;
		int b = v % 256; v /= 256;
		int g = v % 256; v /= 256;
		ColorRGBA(v, g, b);
	}

	void setHSV(const vec3f& t)
	{
		const float piOver3 = 1.0471975512;
		float c = t.z * t.y; // 1
		float x = (1 - abs(fmod(t.x / piOver3, 2) - 1)) * c; // 1
		float m = t.z - c;	// 0
		int p = (int)floor(t.x / piOver3) % 6;	// 0

		switch(p)
		{
		case 0: r = c + m; g = x + m; b = m; break;
		case 1: r = x + m; g = c + m; b = m; break;
		case 2: r = m; g = c + m; b = x + m; break;
		case 3: r = m; g = x + m; b = c + m; break;
		case 4: r = x + m; g = m; b = c + m; break;
		default: r = c + m; g = m; b = x + m; break;
		}
	}

	void operator=(const ColorRGBA& o) { r = o.r; g = o.g; b = o.b; a = o.a; }
	void operator+=(const ColorRGBA& o) { r += o.r; g += o.g; b += o.b; a += o.a; a *= 0.5; }
	void operator-=(const ColorRGBA& o) { r -= o.r; g -= o.g; b -= o.b; a += o.a; a *= 0.5; }
	void operator*=(const ColorRGBA& o) { r *= o.r; g *= o.g; b *= o.b; a += o.a; a *= 0.5; }
	void operator/=(const ColorRGBA& o) { r /= o.r; g /= o.g; b /= o.b; a += o.a; a *= 0.5; }

	ColorRGBA operator+(const ColorRGBA& o) { ColorRGBA p = *this; p += o; return p; }
	ColorRGBA operator-(const ColorRGBA& o) { ColorRGBA p = *this; p -= o; return p; }
	ColorRGBA operator*(const ColorRGBA& o) { ColorRGBA p = *this; p *= o; return p; }
	ColorRGBA operator/(const ColorRGBA& o) { ColorRGBA p = *this; p /= o; return p; }

	void operator=(const vec4f& o) { *this = o; }
	void operator+=(const vec4f& o) { *this += o; a *= 0.5; }
	void operator-=(const vec4f& o) { *this -= o; a *= 0.5; }
	void operator*=(const vec4f& o) { *this *= o; a *= 0.5; }
	void operator/=(const vec4f& o) { *this /= o; a *= 0.5; }

	vec4f operator+(const vec4f& o) { return *this + o; }
	vec4f operator-(const vec4f& o) { return *this + o; }
	vec4f operator*(const vec4f& o) { return *this + o; }
	vec4f operator/(const vec4f& o) { return *this + o; }

	ColorRGBA operator - () { return ColorRGBA(-r, -g, -b, -a); }

	bool operator < (ColorRGBA const& o) { return r < o.r&& g < o.g&& b < o.b&& a < o.a; }
	bool operator > (ColorRGBA const& o) { return r > o.r && g > o.g && b > o.b && a > o.a; }
	bool operator <= (ColorRGBA const& o) { return r <= o.r && g <= o.g && b <= o.b && a <= o.a; }
	bool operator >= (ColorRGBA const& o) { return r >= o.r && g >= o.g && b >= o.b && a >= o.a; }
	bool operator == (ColorRGBA const& o) { return r == o.r && g == o.g && b == o.b && a == o.a; }
	bool operator != (ColorRGBA const& o) { return r != o.r && g != o.g && b != o.b && a != o.a; }

	operator vec3f() { return vec3f(r, g, b); }
	operator vec4f() { return vec4f(r, g, b, a); }
	/*vec3f getHSV()
	{
		const float piOver3 = 1.0471975512;
		float cmin = min(min(r, g), b);
		int t;
		if (r > g) {
		}
	}*/
};

std::ostream& operator<<(std::ostream& os, const ColorRGBA& o) { os << "ColorRGBA(" << o.r << "," << o.g << o.b << "," << o.a << ")"; return os; }

namespace Color
{
	float Similarity(ColorRGB a, ColorRGB b) { vec3f c = abs(a - b); return (c.x + c.y + c.z) / 3.0f; }
	float Similarity(ColorRGBA a, ColorRGBA b) { vec4f c = abs(vec4f(a - b)); return(c.x + c.y + c.z + c.w) / 4.0f; }

	ColorRGB HSV(const vec3f& t)
	{
		const float piOver3 = 1.0471975512;
		float c = t.z * t.y; // 1
		float x = (1 - abs(fmod(t.x / piOver3, 2) - 1)) * c; // 1
		float m = t.z - c;	// 0
		int p = (int)floor(t.x / piOver3) % 6;	// 0

		switch(p)
		{
		case 0:  return ColorRGB(c + m, x + m, m);
		case 1:  return ColorRGB(x + m, c + m, m);
		case 2:  return ColorRGB(m, c + m, x + m);
		case 3:  return ColorRGB(m, x + m, c + m);
		case 4:  return ColorRGB(x + m, m, c + m);
		default: return ColorRGB(c + m, m, x + m);
		}
	}
	inline ColorRGB HSV(float a, float b, float c)
	{
		return HSV(vec3f(a, b, c));
	}

	ColorRGBA mix(const ColorRGBA& c, const ColorRGBA& d, const float& k)
	{
		float l = 1 - k;
		return ColorRGBA(
			c.r * l + d.r * k,
			c.g * l + d.g * k,
			c.b * l + d.b * k,
			c.a * l + d.a * k);
	}

	ColorRGBA mix(const ColorRGBA& c, const ColorRGBA& d)
	{
		return mix(c, d, 0.5);
	}
}
/*Color overlay(const Color& a, const Color& b)
{
	float l = 1 - a.a;
	float o = a.a + b.a * l;
	return Color((a.r*a.a + b.r*b.a) / o, (a.g * a.a + b.g * b.a) / o, (a.b * a.a + b.b * b.a) / o, o);
}*/