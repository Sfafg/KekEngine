#pragma once
#include <iostream>
#include <sstream>
#include <type_traits>
#include <string_view>
#include "Vector.h"

namespace Kek
{
	// Syntax trick to get r, g, b, a aliases for array values
	template <unsigned int TSize, typename T>
	struct color_union
	{
		color_union() {}
		T value[TSize];
	};
	template <typename T>
	struct color_union<1, T>
	{
		color_union() {}
		union
		{
			T value[1];
			struct
			{
				T bw;
			};
		};
	};
	template <typename T>
	struct color_union<2, T>
	{
		color_union() {}
		union
		{
			T value[2];
			struct
			{
				T r, g;
			};
		};
	};
	template <typename T>
	struct color_union<3, T>
	{
		color_union() {}
		union
		{
			T value[3];
			struct
			{
				T r, g, b;
			};
		};
	};
	template <typename T>
	struct color_union<4, T>
	{
		color_union() {}
		union
		{
			T value[4];
			struct
			{
				T r, g, b, a;
			};
		};
	};

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
			switch (p)
			{
			case 0:
				r = c + m;
				g = x + m;
				b = m;
				break;
			case 1:
				r = x + m;
				g = c + m;
				b = m;
				break;
			case 2:
				r = m;
				g = c + m;
				b = x + m;
				break;
			case 3:
				r = m;
				g = x + m;
				b = c + m;
				break;
			case 4:
				r = x + m;
				g = m;
				b = c + m;
				break;
			default:
				r = c + m;
				g = m;
				b = x + m;
				break;
			}

			float mult = 255.0f;
			if constexpr (std::is_floating_point<T>::value)
				mult = 1.0f / mult;

			if constexpr (TSize >= 1)
				this->value[0] = r * mult;
			if constexpr (TSize >= 2)
				this->value[1] = g * mult;
			if constexpr (TSize >= 3)
				this->value[2] = b * mult;
			if constexpr (TSize >= 4)
				this->value[3] = 255.0f * mult;

			return *this;
		}

		template <typename TO>
		void operator+=(const TO &v)
		{
			for (T &val : this->value)
				val += v;
		}
		template <typename TO>
		void operator-=(const TO &v)
		{
			for (T &val : this->value)
				val -= v;
		}
		template <typename TO>
		void operator*=(const TO &v)
		{
			for (T &val : this->value)
				val *= v;
		}
		template <typename TO>
		void operator/=(const TO &v)
		{
			for (T &val : this->value)
				val /= v;
		}

		template <typename TO>
		void operator+=(const col<TSize, TO> &v)
		{
			for (int i = 0; i < TSize; i++)
				operator[](i) += v[i];
		}
		template <typename TO>
		void operator-=(const col<TSize, TO> &v)
		{
			for (int i = 0; i < TSize; i++)
				operator[](i) -= v[i];
		}
		template <typename TO>
		void operator*=(const col<TSize, TO> &v)
		{
			for (int i = 0; i < TSize; i++)
				operator[](i) *= v[i];
		}
		template <typename TO>
		void operator/=(const col<TSize, TO> &v)
		{
			for (int i = 0; i < TSize; i++)
				operator[](i) /= v[i];
		}

		template <typename TO>
		col<TSize, T> operator+(const TO &v) const
		{
			col<TSize, T> col(*this);
			col += v;
			return col;
		}
		template <typename TO>
		col<TSize, T> operator-(const TO &v) const
		{
			col<TSize, T> col(*this);
			col -= v;
			return col;
		}
		template <typename TO>
		col<TSize, T> operator*(const TO &v) const
		{
			col<TSize, T> col(*this);
			col *= v;
			return col;
		}
		template <typename TO>
		col<TSize, T> operator/(const TO &v) const
		{
			col<TSize, T> col(*this);
			col /= v;
			return col;
		}

		template <typename TO>
		col<TSize, T> operator+(const col<TSize, TO> &v) const
		{
			col<TSize, T> col(*this);
			col += v;
			return col;
		}
		template <typename TO>
		col<TSize, T> operator-(const col<TSize, TO> &v) const
		{
			col<TSize, T> col(*this);
			col -= v;
			return col;
		}
		template <typename TO>
		col<TSize, T> operator*(const col<TSize, TO> &v) const
		{
			col<TSize, T> col(*this);
			col *= v;
			return col;
		}
		template <typename TO>
		col<TSize, T> operator/(const col<TSize, TO> &v) const
		{
			col<TSize, T> col(*this);
			col /= v;
			return col;
		}

		col<TSize, T> operator-() const
		{
			col<TSize, T> col(*this);
			for (T &val : col.value)
				val = -val;
			return col;
		}

		bool operator==(const T &v) const
		{
			for (T &val : this->value)
				if (val != v)
					return false;
			return true;
		}
		bool operator!=(const T &v) const
		{
			for (T &val : this->value)
				if (val == v)
					return false;
			return true;
		}
		bool operator<(const T &v) const
		{
			for (T &val : this->value)
				if (val >= v)
					return false;
			return true;
		}
		bool operator>(const T &v) const
		{
			for (T &val : this->value)
				if (val <= v)
					return false;
			return true;
		}
		bool operator<=(const T &v) const
		{
			for (T &val : this->value)
				if (val > v)
					return false;
			return true;
		}
		bool operator>=(const T &v) const
		{
			for (T &val : this->value)
				if (val < v)
					return false;
			return true;
		}

		bool operator==(const col<TSize, T> &v) const
		{
			for (int i = 0; i < TSize; i++)
				if (operator[](i) != v[i])
					return false;
			return true;
		}
		bool operator!=(const col<TSize, T> &v) const
		{
			for (int i = 0; i < TSize; i++)
				if (operator[](i) == v[i])
					return false;
			return true;
		}
		bool operator<(const col<TSize, T> &v) const
		{
			for (int i = 0; i < TSize; i++)
				if (operator[](i) >= v[i])
					return false;
			return true;
		}
		bool operator>(const col<TSize, T> &v) const
		{
			for (int i = 0; i < TSize; i++)
				if (operator[](i) <= v[i])
					return false;
			return true;
		}
		bool operator<=(const col<TSize, T> &v) const
		{
			for (int i = 0; i < TSize; i++)
				if (operator[](i) > v[i])
					return false;
			return true;
		}
		bool operator>=(const col<TSize, T> &v) const
		{
			for (int i = 0; i < TSize; i++)
				if (operator[](i) < v[i])
					return false;
			return true;
		}

		T &operator[](unsigned int index) { return this->value[index]; }
		const T &operator[](unsigned int index) const { return this->value[index]; }

		col() {}
		col(const T *data) { memcpy(this->value, data, sizeof(T) * TSize); }
		col(T const (&data)[TSize]) { memcpy(this->value, data, TSize * sizeof(T)); }
		col(std::initializer_list<T> data) { memcpy(this->value, data.begin(), TSize * sizeof(T)); }
		explicit col(int v)
		{
			T mult = 1.0f;
			if constexpr (std::is_floating_point<T>::value)
				mult = 1.0f / 255.0f;

			if constexpr (TSize >= 1)
				this->value[0] = ((v >> 16) & 0xFF) * mult;
			if constexpr (TSize >= 2)
				this->value[1] = ((v >> 8) & 0xFF) * mult;
			if constexpr (TSize >= 3)
				this->value[2] = (v & 0xFF) * mult;
			if constexpr (TSize >= 4)
				this->value[3] = 255.0f * mult;
		}
		explicit col(const std::string_view &c)
		{
			std::stringstream ss;
			ss << std::hex << c;
			int v;
			ss >> v;
			*this = col(v);
		}

		template <unsigned int OTSize, typename OT>
		void operator=(const col<OTSize, OT> &o)
		{
			float mult = 255.0f;
			if constexpr (std::is_floating_point<T>::value == std::is_floating_point<OT>::value)
				mult = 1.0f;
			else if (std::is_floating_point<T>::value)
				mult = 1.0f / mult;

			constexpr int min = OTSize < TSize ? OTSize : TSize;
			for (int i = 0; i < min; i++)
				this->value[i] = o[i] * mult;
		}
		template <unsigned int OTSize, typename OT>
		col(const col<OTSize, OT> &o)
		{
			float mult = 255.0f;
			if constexpr (std::is_floating_point<T>::value == std::is_floating_point<OT>::value)
				mult = 1.0f;
			else if (std::is_floating_point<T>::value)
				mult = 1.0f / mult;

			constexpr int min = OTSize < TSize ? OTSize : TSize;
			int i = 0;
			for (i; i < min; i++)
				(*this)[i] = o[i] * mult;

			constexpr int max = OTSize > TSize ? OTSize : TSize;
			for (i; i < max; i++)
				(*this)[i] = 0;
		}

		col(const vec<TSize, T> &vec) { memcpy(this->value, vec.value, TSize * sizeof(T)); }
		operator vec<TSize, T>() const
		{
			vec<TSize, T> vec;
			for (int i = 0; i < TSize; i++)
				vec[i] = this->value[i];
			return vec;
		}
	};
	template <unsigned int TSize, typename T>
	std::ostream &operator<<(std::ostream &os, const col<TSize, T> &o)
	{
		os << "col" << TSize << typeid(T).name()[0] << "(";

		if constexpr (std::_Is_character<T>::value)
		{
			for (int i = 0; i < TSize - 1; i++)
				os << (int)o.value[i] << ", ";
			os << (int)o.value[TSize - 1] << ") ";
		}
		else
		{
			for (int i = 0; i < TSize - 1; i++)
				os << o.value[i] << ", ";
			os << o.value[TSize - 1] << ") ";
		}

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
		col<TSize, T> Mix(const col<TSize, T> &c, const col<TSize, T> &d, float k)
		{
			return Maths::Lerp(c, d, k);
		}
		template <unsigned int TSize, typename T>
		float Similarity(const col<TSize, T> &a, const col<TSize, T> &b)
		{
			if constexpr (std::is_unsigned<T>::value)
			{
				if (a < b)
					return vec<TSize, T>(b - a).Mag();
				return vec<TSize, T>(a - b).Mag();
			}
			else
			{
				return vec<TSize, T>(a - b).Mag();
			}
		}
	}

	struct rgb : public col<3>
	{
		rgb();
		rgb(float r, float g, float b);
		explicit rgb(int v);
		explicit rgb(const std::string_view &v);
		template <unsigned int TSize, typename T>
		rgb(const col<TSize, T> &v) : col(v) {}
	};
	struct rgba : public col<4>
	{
		rgba();
		rgba(float r, float g, float b, float a);
		explicit rgba(int v);
		explicit rgba(const std::string_view &v);
		template <unsigned int TSize, typename T>
		rgba(const col<TSize, T> &v) : col(v) {}
	};

	struct rgb8i : public col<3, unsigned char>
	{
		rgb8i();
		rgb8i(unsigned char r, unsigned char g, unsigned char b);
		explicit rgb8i(int v);
		explicit rgb8i(const std::string_view &v);
		template <unsigned int TSize, typename T>
		rgb8i(const col<TSize, T> &v) : col(v) {}
	};
	struct rgba8i : public col<4, unsigned char>
	{
		rgba8i();
		rgba8i(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		explicit rgba8i(int v);
		explicit rgba8i(const std::string_view &v);
		template <unsigned int TSize, typename T>
		rgba8i(const col<TSize, T> &v) : col(v) {}
	};
}