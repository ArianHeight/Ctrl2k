#pragma once
#include <ostream>
#include "basicmath.h"

/*

The rqm base namespace includes everything used for general computer graphics based vector math.
This includes things like vectors of size 1-4, matrix 2x2, 3x3, 4x4(maybe 4x3 as well), and quaternions.

Here we are concerned about speed mostly, and nothing else.

This file is just for the vectors of size 1 to 4.
We are doing element-wise multiplication and division with vectors.

It is possible to divide by 0 using the normalize methods here.

*/

namespace rqm
{
	template <typename T>
	union vec2_base
	{
		struct { T x; T y; };
		struct { T s; T t; };
		struct { T u; T v; };
		T data[2];

		vec2_base() {};
		vec2_base(const T& val) : x(val), y(val) {};
		vec2_base(const T& val0, const T& val1) : x(val0), y(val1) {}
		vec2_base(const vec2_base<T>& other) : x(other.x), y(other.y) {}

		inline T magnitude_squared() const { return x * x + y * y; }
		inline T magnitude() const { return rqm::sqrt(magnitude_squared()); }
		T normalize()
		{
			T mag = magnitude();
			x /= mag;
			y /= mag;
			return mag;
		}
		void normalize_no_return()
		{
			T mag = magnitude();
			x /= mag;
			y /= mag;
		}

		vec2_base<T> operator+() const { return { x, y }; }
		vec2_base<T> operator-() const { return { -x, -y }; }

		vec2_base<T> operator+(const vec2_base<T>& other) const { return { x + other.x, y + other.y }; }
		vec2_base<T> operator-(const vec2_base<T>& other) const { return { x - other.x, y - other.y }; }
		vec2_base<T> operator*(const vec2_base<T>& other) const { return { x * other.x, y * other.y }; }
		vec2_base<T> operator/(const vec2_base<T>& other) const { return { x / other.x, y / other.y }; }
		vec2_base<T> operator+(const T& val) const { return { x + val, y + val }; }
		vec2_base<T> operator-(const T& val) const { return { x - val, y - val }; }
		vec2_base<T> operator*(const T& val) const { return { x * val, y * val }; }
		vec2_base<T> operator/(const T& val) const { return { x / val, y / val }; }

		vec2_base<T>& operator=(const vec2_base<T>& other) { x = other.x; y = other.y; return *this; }
		vec2_base<T>& operator+=(const vec2_base<T>& other) { x += other.x; y += other.y; return *this; }
		vec2_base<T>& operator-=(const vec2_base<T>& other) { x -= other.x; y -= other.y; return *this; }
		vec2_base<T>& operator*=(const vec2_base<T>& other) { x *= other.x; y *= other.y; return *this; }
		vec2_base<T>& operator/=(const vec2_base<T>& other) { x /= other.x; y /= other.y; return *this; }
		vec2_base<T>& operator=(const T& val) { x = val; y = val; return *this; }
		vec2_base<T>& operator+=(const T& val) { x += val; y += val; return *this; }
		vec2_base<T>& operator-=(const T& val) { x -= val; y -= val; return *this; }
		vec2_base<T>& operator*=(const T& val) { x *= val; y *= val; return *this; }
		vec2_base<T>& operator/=(const T& val) { x /= val; y /= val; return *this; }

		bool operator==(const vec2_base<T>& other) { return epsilon_equals(x, other.x) && epsilon_equals(y, other.y); }
	};
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const vec2_base<T>& op1) { os << op1.x << ',' << op1.y; return os; }
	template <typename T>
	inline vec2_base<T> operator+(const T& val, const vec2_base<T>& vec) { return vec + val; }
	template <typename T>
	inline vec2_base<T> operator-(const T& val, const vec2_base<T>& vec) { return { val - vec.x, val - vec.y }; }
	template <typename T>
	inline vec2_base<T> operator*(const T& val, const vec2_base<T>& vec) { return vec * val; }

	template <typename T>
	union vec3_base
	{
		struct { T x; T y; T z; };
		vec2_base<T> xy; // drop the z
		struct { T i; T j; T k; };
		struct { T r; T g; T b; };
		T data[3];

		vec3_base() {};
		vec3_base(const T& val) : x(val), y(val), z(val) {}
		vec3_base(const T& val0, const T& val1, const T& val2) : x(val0), y(val1), z(val2) {}
		vec3_base(const vec3_base<T>& other) : x(other.x), y(other.y), z(other.z) {}
		vec3_base(const vec2_base<T>& base, const T& val = 1) : x(base.x), y(base.y), z(val) {}

		inline T magnitude_squared() const { return x * x + y * y + z * z; }
		inline T magnitude() const { return rqm::sqrt(magnitude_squared()); }
		T normalize()
		{
			T mag = magnitude();
			x /= mag;
			y /= mag;
			z /= mag;
			return mag;
		}
		void normalize_no_return()
		{
			T mag = magnitude();
			x /= mag;
			y /= mag;
			z /= mag;
		}

		vec3_base<T> operator+() const { return { x, y, z }; }
		vec3_base<T> operator-() const { return { -x, -y, -z }; }

		vec3_base<T> operator+(const vec3_base<T>& other) const { return { x + other.x, y + other.y, z + other.z }; }
		vec3_base<T> operator-(const vec3_base<T>& other) const { return { x - other.x, y - other.y, z - other.z }; }
		vec3_base<T> operator*(const vec3_base<T>& other) const { return { x * other.x, y * other.y, z * other.z }; }
		vec3_base<T> operator/(const vec3_base<T>& other) const { return { x / other.x, y / other.y, z / other.z }; }
		vec3_base<T> operator+(const T& val) const { return { x + val, y + val, z + val }; }
		vec3_base<T> operator-(const T& val) const { return { x - val, y - val, z - val }; }
		vec3_base<T> operator*(const T& val) const { return { x * val, y * val, z * val }; }
		vec3_base<T> operator/(const T& val) const { return { x / val, y / val, z / val }; }

		vec3_base<T>& operator=(const vec3_base<T>& other) { x = other.x; y = other.y; z = other.z; return *this; }
		vec3_base<T>& operator+=(const vec3_base<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
		vec3_base<T>& operator-=(const vec3_base<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
		vec3_base<T>& operator*=(const vec3_base<T>& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
		vec3_base<T>& operator/=(const vec3_base<T>& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
		vec3_base<T>& operator=(const T& val) { x = val; y = val; z = val; return *this; }
		vec3_base<T>& operator+=(const T& val) { x += val; y += val; z += val; return *this; }
		vec3_base<T>& operator-=(const T& val) { x -= val; y -= val; z -= val; return *this; }
		vec3_base<T>& operator*=(const T& val) { x *= val; y *= val; z *= val; return *this; }
		vec3_base<T>& operator/=(const T& val) { x /= val; y /= val; z /= val; return *this; }

		bool operator==(const vec3_base<T>& other) { return epsilon_equals(x, other.x) && epsilon_equals(y, other.y) && epsilon_equals(z, other.z); }
	};
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const vec3_base<T>& op1) { os << op1.x << ',' << op1.y << ',' << op1.z; return os; }
	template <typename T>
	inline vec3_base<T> operator+(const T& val, const vec3_base<T>& vec) { return vec + val; }
	template <typename T>
	inline vec3_base<T> operator-(const T& val, const vec3_base<T>& vec) { return { val - vec.x, val - vec.y, val - vec.z }; }
	template <typename T>
	inline vec3_base<T> operator*(const T& val, const vec3_base<T>& vec) { return vec * val; }


	template <typename T>
	union vec4_base
	{
		struct { T x; T y; T z; T w; };
		vec2_base<T> xy; // drop the zw
		vec3_base<T> xyz; // drop the w
		struct { T i; T j; T k; T scalar; }; // for quats, but please type cast to a quat type before using
		vec3_base<T> ijk; // drop the scalar
		struct { T r; T g; T b; T a; };
		vec3_base<T> rgb; // drop the a
		struct { T s; T t; T u; T v; };
		struct { vec2_base<T> st; vec2_base<T> uv; };
		T data[4];

		vec4_base() {};
		vec4_base(const T& val) : x(val), y(val), z(val), w(val) {}
		vec4_base(const T& val0, const T& val1, const T& val2, const T& val3) : x(val0), y(val1), z(val2), w(val3) {}
		vec4_base(const vec4_base<T>& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
		vec4_base(const vec3_base<T>& base, const T& val = 1) : x(base.x), y(base.y), z(base.z), w(val) {}

		inline T magnitude_squared() const { return x * x + y * y + z * z + w * w; }
		inline T magnitude() const { return rqm::sqrt(magnitude_squared()); }
		T normalize()
		{
			T mag = magnitude();
			x /= mag;
			y /= mag;
			z /= mag;
			w /= mag;
			return mag;
		}
		void normalize_no_return()
		{
			T mag = magnitude();
			x /= mag;
			y /= mag;
			z /= mag;
			w /= mag;
		}

		vec4_base<T> operator+() const { return { x, y, z, w }; }
		vec4_base<T> operator-() const { return { -x, -y, -z, -w }; }

		vec4_base<T> operator+(const vec4_base<T>& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		vec4_base<T> operator-(const vec4_base<T>& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		vec4_base<T> operator*(const vec4_base<T>& other) const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }
		vec4_base<T> operator/(const vec4_base<T>& other) const { return { x / other.x, y / other.y, z / other.z, w / other.w }; }
		vec4_base<T> operator+(const T& val) const { return { x + val, y + val, z + val, w + val }; }
		vec4_base<T> operator-(const T& val) const { return { x - val, y - val, z - val, w - val }; }
		vec4_base<T> operator*(const T& val) const { return { x * val, y * val, z * val, w * val }; }
		vec4_base<T> operator/(const T& val) const { return { x / val, y / val, z / val, w / val }; }

		vec4_base<T>& operator=(const vec4_base<T>& other) { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
		vec4_base<T>& operator+=(const vec4_base<T>& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
		vec4_base<T>& operator-=(const vec4_base<T>& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
		vec4_base<T>& operator*=(const vec4_base<T>& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
		vec4_base<T>& operator/=(const vec4_base<T>& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
		vec4_base<T>& operator=(const T& val) { x = val; y = val; z = val; w = val; return *this; }
		vec4_base<T>& operator+=(const T& val) { x += val; y += val; z += val; w += val; return *this; }
		vec4_base<T>& operator-=(const T& val) { x -= val; y -= val; z -= val; w -= val; return *this; }
		vec4_base<T>& operator*=(const T& val) { x *= val; y *= val; z *= val; w *= val; return *this; }
		vec4_base<T>& operator/=(const T& val) { x /= val; y /= val; z /= val; w /= val; return *this; }

		bool operator==(const vec4_base<T>& other)
		{
			return epsilon_equals(x, other.x) && epsilon_equals(y, other.y) && epsilon_equals(z, other.z) && epsilon_equals(w, other.w);
		}
	};
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const vec4_base<T>& op1) { os << op1.x << ',' << op1.y << ',' << op1.z << ',' << op1.w; return os; }
	template <typename T>
	inline vec4_base<T> operator+(const T& val, const vec4_base<T>& vec) { return vec + val; }
	template <typename T>
	inline vec4_base<T> operator-(const T& val, const vec4_base<T>& vec) { return { val - vec.x, val - vec.y, val - vec.z, val - vec.w }; }
	template <typename T>
	inline vec4_base<T> operator*(const T& val, const vec4_base<T>& vec) { return vec * val; }

	typedef float vec1;
	typedef vec2_base<float> vec2;
	typedef vec3_base<float> vec3;
	typedef vec4_base<float> vec4;

	typedef double dvec1;
	typedef vec2_base<double> dvec2;
	typedef vec3_base<double> dvec3;
	typedef vec4_base<double> dvec4;

	typedef int ivec1;
	typedef vec2_base<int> ivec2;
	typedef vec3_base<int> ivec3;
	typedef vec4_base<int> ivec4;

	template <typename T>
	T dot(const vec2_base<T>& op1, const vec2_base<T>& op2) { return op1.x * op2.x + op1.y * op2.y; }
	template <typename T>
	T dot(const vec3_base<T>& op1, const vec3_base<T>& op2) { return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z; }
	template <typename T>
	T dot(const vec4_base<T>& op1, const vec4_base<T>& op2) { return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z + op1.w * op2.w; }
	template <typename T>
	vec3_base<T> cross(const vec3_base<T>& op1, const vec3_base<T>& op2)
	{
		return { op1.y * op2.z - op1.z * op2.y, op1.z * op2.x - op1.x * op2.z, op1.x * op2.y - op1.y * op2.x };
	}
}