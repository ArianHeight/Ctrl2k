#pragma once
#include <cmath>

namespace rqm
{
	// basic

	template <typename T>
	inline T sqrt(const T& val) { return std::sqrt(val); }
	template <typename T>
	inline T acos(const T& val) { return std::acos(val); }

	// trig

	template <typename T>
	inline T sin(const T& val) { return std::sin(val); }

	template <typename T>
	inline T cos(const T& val) { return std::cos(val); }

	const double dpi = 3.1415926535897932384626433832795028841971693993751058209;
	const float pi = 3.141592653589793238462643383f;
	const int ipi = 3;

	const float DEGREE_TO_RAD = pi / 180.0f;
	const float RAD_TO_DEGREE = 1.0f / DEGREE_TO_RAD;

	template <typename T>
	inline T deg2rad(const T& val) { return DEGREE_TO_RAD * val; }
	template <typename T>
	inline T rad2deg(const T& val) { return RAD_TO_DEGREE * val; }
}