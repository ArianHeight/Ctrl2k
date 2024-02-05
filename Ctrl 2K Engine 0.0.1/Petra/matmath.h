#pragma once
#include "vecmath.h"

/*

The rqm base namespace includes everything used for general computer graphics based vector math.
This includes things like vectors of size 1-4, matrix 2x2, 3x3, 4x4(maybe 4x3 as well), and quaternions.

Here we are concerned about speed mostly, and nothing else.

This file is just for the square matrices of sizes 2x2, 3x3, and 4x4.

*/

namespace rqm
{
    template <typename T>
    union mat22_base
    {
        struct { vec2_base<T> x; vec2_base<T> y; };
        struct { vec2_base<T> row0; vec2_base<T> row1; };
        struct { T xx; T xy; T yx; T yy; };
        T data[2][2]; // C++ is row major
        T flat_data[4];

        mat22_base() {}
        mat22_base(const T& val1, const T& val2, const T& val3, const T& val4) : xx(val1), xy(val2), yx(val3), yy(val4) {}
        mat22_base(const vec2_base<T>& vec1, const vec2_base<T>& vec2) : row0(vec1), row1(vec2) {}
        mat22_base(const mat22_base<T>& mat) : row0(mat.row0), row1(mat.row1) {}

        void set_columns(const vec2_base<T>& col0, const vec2_base<T>& col1)
        {
            xx = col0.x;
            xy = col1.x;
            yx = col0.y;
            yy = col1.y;
        }
        void transpose()
        {
            mat22_base<T> mat = { row0, row1 };
            set_columns(mat.row0, mat.row1);
        }

        mat22_base<T> get_transpose() const
        {
            mat22_base<T> mat;
            mat.set_columns(row0, row1);
            return mat;
        }

        mat22_base<T> operator+() const { return { row0, row1 }; }
        mat22_base<T> operator-() const { return { -row0, -row1 }; }

        mat22_base<T> operator+(const mat22_base<T>& other) const { return { row0 + other.row0, row1 + other.row1 }; }
        mat22_base<T> operator-(const mat22_base<T>& other) const { return { row0 - other.row0, row1 - other.row1 }; }
        mat22_base<T> operator*(const mat22_base<T>& other) const
        {
            // TODO: test with benchmark and see if we need to implement strassen's multiplication
            mat22_base<T> out;
            out.xx = xx * other.xx + xy * other.yx;
            out.xy = xx * other.xy + xy * other.yy;
            out.yx = yx * other.xx + yy * other.yx;
            out.yy = yx * other.xy + yy * other.yy;
            return out;
        }
        vec2_base<T> operator*(const vec2_base<T>& other) const
        {
            vec2_base<T> out;
            out.x = xx * other.x + xy * other.y;
            out.y = yx * other.x + yy * other.y;
            return out;
        }
        // division doesn't make sense here
        mat22_base<T> operator+(const T& val) const { return { row0 + val, row1 + val }; }
        mat22_base<T> operator-(const T& val) const { return { row0 - val, row1 - val }; }
        mat22_base<T> operator*(const T& val) const { return { row0 * val, row1 - val }; }
        mat22_base<T> operator/(const T& val) const { return { row0 / val, row0 / val }; }

        mat22_base<T>& operator=(const mat22_base<T>& other) { row0 = other.row0; row1 = other.row1; return *this; }
        mat22_base<T>& operator+=(const mat22_base<T>& other) { row0 += other.row0; row1 += other.row1; return *this; }
        mat22_base<T>& operator-=(const mat22_base<T>& other) { row0 -= other.row1; row1 -= other.row1; return *this; }
        // *= and /= don't make sense here, *= specifically does not indicate if this is A * B or B * A
        mat22_base<T>& operator=(const T& val) { row0 = val; row1 = val; return *this; }
        mat22_base<T>& operator+=(const T& val) { row0 += val; row1 += val; return *this; }
        mat22_base<T>& operator-=(const T& val) { row0 -= val; row1 -= val; return *this; }
        mat22_base<T>& operator*=(const T& val) { row0 *= val; row1 *= val; return *this; }
        mat22_base<T>& operator/=(const T& val) { row0 /= val; row1 /= val; return *this; }
    };
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const mat22_base<T>& op1) { os << '[' << op1.row0 << '|' << op1.row1 << ']'; return os; }
    template <typename T>
    inline mat22_base<T> operator+(const T& val, const mat22_base<T>& mat) { return mat + val; }
    template <typename T>
    inline mat22_base<T> operator-(const T& val, const mat22_base<T>& mat) { return -mat + val; }
    template <typename T>
    inline mat22_base<T> operator*(const T& val, const mat22_base<T>& mat) { return mat * val; }
    template <typename T>
    inline mat22_base<T> gen_mat22_identity() { return { 1, 0, 0, 1 }; }

    typedef mat22_base<float> mat22;
    typedef mat22_base<double> dmat22;
}