#pragma once
#include "quatmath.h"

/*

TODO: test the functions here actually work properly

*/

namespace rqm
{
	template <typename T, typename V>
	void lerp_in_place(const V& start, const V& end, const T& t, V& out)
	{
		out = start * t + end * (1 - t);
	}
	template <typename T, typename V>
	V lerp(const V& start, const V& end, const T& t)
	{
		V out;
		lerp_in_place(start, end, t, out);
		return out;
	}

	// V must have a method with signature void normalize_no_return()
	template <typename T, typename V>
	void nlerp_in_place(const V& start, const V& end, const T& t, V& out)
	{
		lerp_in_place(start, end, t, out);
		out.normalize_no_return();
	}
	template <typename T, typename V>
	V nlerp(const V& start, const V& end, const T& t)
	{
		V out;
		nlerp_in_place(start, end, t, out);
		return out;
	}

	// assumes inputs are already normalized
	template <typename T>
	void slerp_in_place(const quat_base<T>& start, const quat_base<T>& end, const T& t, quat_base<T>& out)
	{
		T theta = acos(dot(start.data, end.data));
		out = ((start * sin((1 - t) * theta)) + (end * sin(t * theta))) / sin(theta);
	}
	template <typename T>
	quat_base<T> slerp(const quat_base<T>& start, const quat_base<T>& end, const T& t)
	{
		quat_base<T> out;
		slerp_in_place(start, end, t, out);
		return out;
	}

	// assumes inputs are already normalized
	// TODO make sure this is right?
	template <typename T>
	void slerp_short_in_place(const quat_base<T>& start, const quat_base<T>& end, const T& t, quat_base<T>& out)
	{
		T d = dot(start.data, end.data);
		T theta = acos(d);
		out = ((start * sin((1 - t) * theta)) * signum_no_zero(d) + (end * sin(t * theta))) / sin(theta);
	}
	template <typename T>
	quat_base<T> slerp_short(const quat_base<T>& start, const quat_base<T>& end, const T& t)
	{
		quat_base<T> out;
		slerp_short_in_place(start, end, t, out);
		return out;
	}
}