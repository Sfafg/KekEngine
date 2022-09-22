#pragma once
#include <iostream>
#include <sstream>
#include <type_traits>
#include <string_view>
#include "Vector.h"

namespace Kek
{
	// Syntax trick to get r, g, b, a aliases for array values
	template <unsigned int TSize, typename T> struct color_union { color_union() {} T value[TSize]; };
	template <typename T> struct color_union<1, T> { color_union() {} union { T value[1]; struct { T bw; }; }; };
	template <typename T> struct color_union<2, T> { color_union() {} union { T value[2]; struct { T r, g; }; }; };
	template <typename T> struct color_union<3, T> { color_union() {} union { T value[3]; struct { T r, g, b; }; }; };
	template <typename T> struct color_union<4, T> { color_union() {} union { T value[4]; struct { T r, g, b, a; }; }; };

	template <unsigned int TSize, typename T = float>
	struct col : public color_union<TSize, T>
	{
		int Size() { return TSize; }
		col SetHSV(float H, float S, float V)
		{
			float c = V * S;
			float x = (1 - abs(fmod(H / Maths::PI / 3.0f, 2) - 1)) * c;
			float m = V - c;
			int p = (int)floor(H / Maths::PI / 3.0f) % 6;

			float r, g, b;
			switch(p)
			{
			case 0: r = c + m; g = x + m; b = m; break;
			case 1: r = x + m; g = c + m; b = m; break;
			case 2: r = m; g = c + m; b = x + m; break;
			case 3: r = m; g = x + m; b = c + m; break;
			case 4: r = x + m; g = m; b = c + m; break;
			default: r = c + m; g = m; b = x + m; break;
			}

			float mult = 255.0f;
			if constexpr(std::is_floating_point<T>::value) mult = 1.0f / mult;

			if constexpr(TSize >= 1) this->value[0] = r * mult;
			if constexpr(TSize >= 2) this->value[1] = g * mult;
			if constexpr(TSize >= 3) this->value[2] = b * mult;
			if constexpr(TSize >= 4) this->value[3] = 255.0f * mult;

			return *this;
		}

		void operator += (const T& v) { for(T& val : this->value) val += v; }
		void operator -= (const T& v) { for(T& val : this->value) val -= v; }
		void operator *= (const T& v) { for(T& val : this->value) val *= v; }
		void operator /= (const T& v) { for(T& val : this->value) val /= v; }

		void operator += (const col<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) += v[i]; }
		void operator -= (const col<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) -= v[i]; }
		void operator *= (const col<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) *= v[i]; }
		void operator /= (const col<TSize, T>& v) { for(int i = 0; i < TSize; i++) operator[](i) /= v[i]; }

		col<TSize, T> operator + (const T& v) const { col<TSize, T> col(*this); col += v; return col; }
		col<TSize, T> operator - (const T& v) const { col<TSize, T> col(*this); col -= v; return col; }
		col<TSize, T> operator * (const T& v) const { col<TSize, T> col(*this); col *= v; return col; }
		col<TSize, T> operator / (const T& v) const { col<TSize, T> col(*this); col /= v; return col; }

		col<TSize, T> operator + (const col<TSize, T>& v) const { col<TSize, T> col(*this); col += v; return col; }
		col<TSize, T> operator - (const col<TSize, T>& v) const { col<TSize, T> col(*this); col -= v; return col; }
		col<TSize, T> operator * (const col<TSize, T>& v) const { col<TSize, T> col(*this); col *= v; return col; }
		col<TSize, T> operator / (const col<TSize, T>& v) const { col<TSize, T> col(*this); col /= v; return col; }

		col<TSize, T> operator - () const { col<TSize, T> col(*this);  for(T& val : col.value) val = -val; return col; }

		bool operator ==(const T& v) const { for(T& val : this->value) if(val != v) return false; return true; }
		bool operator !=(const T& v) const { for(T& val : this->value) if(val == v) return false; return true; }
		bool operator < (const T& v) const { for(T& val : this->value) if(val >= v) return false; return true; }
		bool operator > (const T& v) const { for(T& val : this->value) if(val <= v) return false; return true; }
		bool operator <=(const T& v) const { for(T& val : this->value) if(val > v) return false; return true; }
		bool operator >=(const T& v) const { for(T& val : this->value) if(val < v) return false; return true; }

		bool operator == (const col<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) != v[i]) return false; return true; }
		bool operator != (const col<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) == v[i]) return false; return true; }
		bool operator <  (const col<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) >= v[i]) return false; return true; }
		bool operator >  (const col<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) <= v[i]) return false; return true; }
		bool operator <= (const col<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) > v[i]) return false; return true; }
		bool operator >= (const col<TSize, T>& v) const { for(int i = 0; i < TSize; i++) if(operator[](i) < v[i]) return false; return true; }

		T& operator[](unsigned int index) { return this->value[index]; }
		const T& operator[](unsigned int index) const { return this->value[index]; }
		operator T* () { return &(this->value[0]); }
		operator const T* () const { return &(this->value[0]); }

		col() {}
		col(const T* data) { memcpy(this->value, data, sizeof(T) * TSize); }
		col(T const (&data)[TSize]) { memcpy(this->value, data, TSize * sizeof(T)); }
		col(std::initializer_list<T> data) { memcpy(this->value, data.begin(), TSize * sizeof(T)); }
		col(int v)
		{
			float mult = 1.0f;
			if constexpr(std::is_floating_point<T>::value) mult = 1.0f / 255.0f;

			if constexpr(TSize >= 1) this->value[0] = ((v >> 16) & 0xFF) * mult;
			if constexpr(TSize >= 2) this->value[1] = ((v >> 8) & 0xFF) * mult;
			if constexpr(TSize >= 3) this->value[2] = (v & 0xFF) * mult;
			if constexpr(TSize >= 4) this->value[3] = 255.0f * mult;
		}
		col(const std::string_view& c)
		{
			std::stringstream ss;
			ss << std::hex << c;
			int v;
			ss >> v;
			*this = col(v);
		}

		template <unsigned int OTSize, typename OT>
		void operator =(const col<OTSize, OT>& o)
		{
			float mult = 255.0f;
			if constexpr(std::is_floating_point<T>::value == std::is_floating_point<OT>::value) mult = 1.0f;
			else if(std::is_floating_point<T>::value) mult = 1.0f / mult;

			constexpr int min = OTSize < TSize ? OTSize : TSize;
			for(int i = 0; i < min; i++) this->value[i] = o[i] * mult;
		}
		template <unsigned int OTSize, typename OT>
		col(const col<OTSize, OT>& o)
		{
			float mult = 255.0f;
			if constexpr(std::is_floating_point<T>::value == std::is_floating_point<OT>::value) mult = 1.0f;
			else if(std::is_floating_point<T>::value) mult = 1.0f / mult;

			constexpr int min = OTSize < TSize ? OTSize : TSize;
			int i = 0;
			for(i; i < min; i++) (*this)[i] = o[i] * mult;

			constexpr int max = OTSize > TSize ? OTSize : TSize;
			for(i; i < max; i++) (*this)[i] = 0;
		}

		col(const vec<TSize, T>& vec) { memcpy(this->value, vec.value, TSize * sizeof(T)); }
		operator vec<TSize, T>() const { vec<TSize, T> vec; for(int i = 0; i < TSize; i++) vec[i] = this->value[i]; return vec; }
	};
	template <unsigned int TSize, typename T>
	std::ostream& operator<<(std::ostream& os, const col<TSize, T>& o)
	{
		os << "[{LCyan}]<col" << TSize << typeid(T).name()[0] << ">[{LYellow}]<(";

		if constexpr(sizeof(T) == 1)
		{
			for(int i = 0; i < TSize - 1; i++) os << (int)o.value[i] << ", ";
			os << (int)o.value[TSize - 1] << ") ";
		}
		else
		{
			for(int i = 0; i < TSize - 1; i++) os << o.value[i] << ", ";
			os << o.value[TSize - 1] << ") ";
		}
		os << '>';

		return os;
	}

	namespace Color
	{
		template <unsigned int TSize, typename T>
		col<TSize, T> HSV(float H, float S, float V)
		{
			return col<TSize, T>().SetHSV(H, S, V);
		}
		template <unsigned int TSize, typename T>
		col<TSize, T> Mix(const col<TSize, T>& c, const col<TSize, T>& d, float k)
		{
			return Maths::Lerp(c, d, k);
		}
		template <unsigned int TSize, typename T>
		float Similarity(const col<TSize, T>& a, const col<TSize, T>& b)
		{
			if constexpr(std::is_unsigned<T>::value)
			{
				if(a < b)return vec<TSize, T>(b - a).Mag();
				return vec<TSize, T>(a - b).Mag();
			}
			else
			{
				return vec<TSize, T>(a - b).Mag();
			}
		}
	}

	struct colRGB : public col<3>
	{
		colRGB();
		colRGB(float r, float g, float b);
		colRGB(int v);
		colRGB(const std::string_view& v);
		template <unsigned int TSize, typename T>
		colRGB(const col<TSize, T>& v) : col(v) {}
	};
	struct colRGBA : public col<4>
	{
		colRGBA();
		colRGBA(float r, float g, float b, float a);
		colRGBA(int v);
		colRGBA(const std::string_view& v);
		template <unsigned int TSize, typename T>
		colRGBA(const col<TSize, T>& v) : col(v) {}
	};

	struct colRGB8i : public col<3, unsigned char>
	{
		colRGB8i();
		colRGB8i(unsigned char r, unsigned char g, unsigned char b);
		colRGB8i(int v);
		colRGB8i(const std::string_view& v);
		template <unsigned int TSize, typename T>
		colRGB8i(const col<TSize, T>& v) : col(v) {}
	};
	struct colRGBA8i : public col<4, unsigned char>
	{
		colRGBA8i();
		colRGBA8i(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		colRGBA8i(int v);
		colRGBA8i(const std::string_view& v);
		template <unsigned int TSize, typename T>
		colRGBA8i(const col<TSize, T>& v) : col(v) {}
	};
}