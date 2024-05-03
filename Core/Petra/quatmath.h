#pragma once
#include "matmath.h"

/*

The rqm base namespace includes everything used for general computer graphics based vector math.
This includes things like vectors of size 1-4, matrix 2x2, 3x3, 4x4(maybe 4x3 as well), and quaternions.

Here we are concerned about speed mostly, and nothing else.

This file is just for quaternions, these aren't included as a part of the vector union because the multiplication behaviour is different.

*/

namespace rqm
{


template <typename T>
struct quat_base
{
	vec4_base<T> data;

	quat_base() {}
	quat_base(const vec4_base<T>& vec) : data(vec) {}
	quat_base(const T& s, const vec3_base<T>& vec) { data.ijk = vec; data.scalar = s; }
	quat_base(const T& i, const T& j, const T& k, const T& s) : data(i, j, k, s) {}

	inline T magnitude_squared() const { return data.magnitude_squared(); }
	inline T magnitude() const { return data.magnitude(); }
	inline T normalize() { return data.normalize(); }
	inline void normalize_no_return() { data.normalize_no_return(); }

	quat_base<T> operator+() const { return { data }; }
	quat_base<T> operator-() const { return { -data }; }

	quat_base<T> operator+(const quat_base<T>& other) const { return { data + other.data }; }
	quat_base<T> operator-(const quat_base<T>& other) const { return { data - other.data }; }
	quat_base<T> operator*(const quat_base<T>& other) const
	{
		T aa, ab, ac, ad, ba, bb, bc, bd, ca, cb, cc, cd, da, db, dc, dd;
		bb = data.i * other.data.i; // i^2 = -1
		bc = data.i * other.data.j; // ij = k
		bd = data.i * other.data.k; // ik = -j
		ba = data.i * other.data.scalar; // i
		cb = data.j * other.data.i; // ji = -k
		cc = data.j * other.data.j; // j^2 = -1
		cd = data.j * other.data.k; // jk = i
		ca = data.j * other.data.scalar; // j
		db = data.k * other.data.i; // ki = j
		dc = data.k * other.data.j; // kj = -i
		dd = data.k * other.data.k; // k^2 = -1
		da = data.k * other.data.scalar; // k
		ab = data.scalar * other.data.i;
		ac = data.scalar * other.data.j;
		ad = data.scalar * other.data.k;
		aa = data.scalar * other.data.scalar;
		return quat_base<T>(ab + ba + cd - dc, ac - bd + ca + db, ad + bc - cb + da, aa - bb - cc - dd);
	}
	// no default division here, pls multiply by inverse
	quat_base<T> operator+(const T& val) const { return quat_base<T>(data.scalar + val, data.ijk); }
	quat_base<T> operator-(const T& val) const { return quat_base<T>(data.scalar - val, data.ijk); }
	quat_base<T> operator*(const T& val) const { return { data * val }; }
	quat_base<T> operator/(const T& val) const { return { data / val }; }

	quat_base<T>& operator=(const quat_base<T>& other) { data = other.data; return *this; }
	quat_base<T>& operator+=(const quat_base<T>& other) { data += other.data; return *this; }
	quat_base<T>& operator-=(const quat_base<T>& other) { data -= other.data; return *this; }
	quat_base<T>& operator+=(const T& val) const { data.scalar += val; return *this; }
	quat_base<T>& operator-=(const T& val) const { data.scalar -= val; return *this; }
	quat_base<T>& operator*=(const T& val) const { data *= val; return *this; }
	quat_base<T>& operator/=(const T& val) const { data /= val; return *this; }

	inline quat_base<T> get_conjugate() const { return rqm::quat_base<T>(data.scalar, -data.ijk); }
	quat_base<T> get_inverse() const { return get_conjugate() / magnitude_squared(); }
	// TODO to_matrix()

	void rotate_in_place(quat_base<T>& p) const
	{
		quat_base<T> inv = get_inverse();
		p = *this * p * inv;
	}
	void rotate_in_place(vec3_base<T>& v) const
	{
		quat_base<T> p = quat_base<T>(0, v);
		rotate_in_place(p);
		v = p.data.ijk;
	}
	void rotate_in_place(vec4_base<T>& v) const
	{
		T s = v.scalar;
		rotate_in_place(as_quat(v));
		v.scalar = s;
	}
	quat_base<T> rotate(quat_base<T> p) const
	{
		rotate_in_place(p);
		return p;
	}
	vec3_base<T> rotate(const vec3_base<T>& vec) const
	{
		quat_base<T> p = quat_base<T>(0, vec);
		rotate_in_place(p);
		return p.data.ijk;
	}
	vec4_base<T> rotate(vec4_base<T> vec) const
	{
		rotate_in_place(vec);
		return vec;
	}

	inline bool operator==(const quat_base<T>& other) const { return data == other.data; }
};
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const quat_base<T>& op1) { os << op1.data; return os; }
template <typename T>
inline quat_base<T> operator+(const T& val, const quat_base<T>& q) { return q + val; }
template <typename T>
inline quat_base<T> operator-(const T& val, const quat_base<T>& q) { return rqm::quat_base<T>(-q.data) + val; }
template <typename T>
inline quat_base<T> operator*(const T& val, const quat_base<T>& q) { return { q * val }; }

typedef quat_base<float> quat;
typedef quat_base<double> dquat;

template<typename T>
quat_base<T>& as_quat(vec4_base<T>& vec) { return reinterpret_cast<quat_base<T>&>(vec); }

template<typename T>
quat_base<T> gen_quat_angle_about_axis_rad(T theta_rad, vec3_base<T> axis)
{
	axis.normalize();
	theta_rad /= 2;
	return quat_base<T>(rqm::cos(theta_rad), rqm::sin(theta_rad) * axis);
}
template<typename T>
inline quat_base<T> gen_quat_angle_about_axis_deg(T theta_deg, const vec3_base<T>& axis)
{
	return gen_quat_angle_about_axis_rad(deg2rad(theta_deg), axis);
}

template<typename T>
mat33_base<T> quat_to_mat33(const quat_base<T>& q)
{
	T m, im, jm, km, si, sj, sk, ii, ij, ik, jj, jk, kk;
	m = 2 / q.data.magnitude_squared();
	im = q.data.i * m;
	jm = q.data.j * m;
	km = q.data.k * m;
	si = q.data.scalar * im;
	sj = q.data.scalar * jm;
	sk = q.data.scalar * km;
	ii = q.data.i * im;
	ij = q.data.i * jm;
	ik = q.data.i * km;
	jj = q.data.j * jm;
	jk = q.data.j * km;
	kk = q.data.k * km;
	return { { 1 - jj - kk, ij - sk, ik + sj }, { ij + sk, 1 - ii - kk, jk - si }, { ik - sj, ik + si, 1 - ii - jj } };
}
template<typename T>
quat_base<T> mat33_to_quat(const mat33_base<T>& m)
{
	T t, r, s;
	t = m.get_trace();
	r = rqm::sqrt(1 + t);
	s = 1 / (2 * r);
	return { s * (m.zy - m.yz), s * (m.xz - m.zx), s * (m.yx - m.xy), r / 2 };
}


}