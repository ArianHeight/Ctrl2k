#pragma once
#include <cmath>

namespace rqm
{
// basic

template <typename T> constexpr T EPSILON;
template <> constexpr float EPSILON<float> = FLT_EPSILON;
template <> constexpr double EPSILON<double> = DBL_EPSILON;
template <> constexpr long double EPSILON<long double> = LDBL_EPSILON;
template <> constexpr int EPSILON<int> = 0;

// return -1 for negative numbers, 0 for 0, 1 for positive numbers
template <typename T>
T signum(const T& val) { return (T)(val > 0) - (T)(val < 0); }

// return -1 for negative numbers, 1 otherwise
template <typename T>
T signum_no_zero(const T& val) { return (T)(val >= 0) - (T)(val < 0); }

template <typename T>
inline T abs(const T& val) { return std::abs(val); }

template <typename T>
inline T min(const T& val0, const T& val1) { return std::min(val0, val1); }
template <typename T>
inline T max(const T& val0, const T& val1) { return std::max(val0, val1); }

template <typename T>
inline bool epsilon_equals(const T& val0, const T& val1) { return abs(val1 - val0) <= EPSILON<T>; }

template <typename T>
inline T sqrt(const T& val) { return std::sqrt(val); }

// trig

template <typename T>
inline T acos(const T& val) { return std::acos(val); }
template <typename T>
inline T sin(const T& val) { return std::sin(val); }
template <typename T>
inline T cos(const T& val) { return std::cos(val); }

template <typename T> constexpr T PI;
template <> constexpr float PI<float> = 3.141592653f;
template <> constexpr double PI<double> = 3.14159265358979323846;
template <> constexpr double PI<long double> = 3.141592653589793238L;
template <> constexpr int PI<int> = 3;

template <typename T> constexpr T DEGREE_TO_RAD;
template <> constexpr float DEGREE_TO_RAD<float> = PI<float> / 180.0f;
template <> constexpr double DEGREE_TO_RAD<double> = PI<double> / 180.0;
template <> constexpr long double DEGREE_TO_RAD<long double> = PI<long double> / 180.0L;
template <> constexpr int DEGREE_TO_RAD<int> = PI<int> / 180;

template <typename T> constexpr T RAD_TO_DEGREE;
template <> constexpr float RAD_TO_DEGREE<float> = 180.0f / PI<float>;
template <> constexpr double RAD_TO_DEGREE<double> = 180.0 / PI<double>;
template <> constexpr long double RAD_TO_DEGREE<long double> = 180.0 / PI<long double>;
template <> constexpr int RAD_TO_DEGREE<int> = 180 / PI<int>;

template <typename T>
inline T deg2rad(const T& val) { return DEGREE_TO_RAD<T> *val; }
template <typename T>
inline T rad2deg(const T& val) { return RAD_TO_DEGREE<T> *val; }


}