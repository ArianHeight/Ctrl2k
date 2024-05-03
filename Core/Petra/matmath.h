#pragma once
#include "vecmath.h"
#include <vector>

/*

The rqm base namespace includes everything used for general computer graphics based vector math.
This includes things like vectors of size 1-4, matrix 2x2, 3x3, 4x4(maybe 4x3 as well), and quaternions.

Here we are concerned about speed mostly, and nothing else.

This file is just for the square matrices of sizes 2x2, 3x3, and 4x4.

Note: The constructor for all mat types here are generating from rows! Use gen from col functions to create mats from columns.

*/

//#define RQM_MATRIX_DOT_IMPL

namespace rqm
{


template <typename T>
union mat22_base
{
    struct { vec2_base<T> x; vec2_base<T> y; };
    struct { vec2_base<T> row0; vec2_base<T> row1; };
    struct { T xx; T xy; T yx; T yy; };
    vec2_base<T> data[2]; // C++ is row major
    T flat_data[4];

    mat22_base() {}
    mat22_base(const T& val0, const T& val1, const T& val2, const T& val3) : xx(val0), xy(val1), yx(val2), yy(val3) {}
    mat22_base(const vec2_base<T>& vec0, const vec2_base<T>& vec1) : row0(vec0), row1(vec1) {}
    mat22_base(const mat22_base<T>& mat) : row0(mat.row0), row1(mat.row1) {}

    void set_columns(const vec2_base<T>& col0, const vec2_base<T>& col1)
    {
        xx = col0.x; xy = col1.x;
        yx = col0.y; yy = col1.y;
    }

    void transpose()
    {
        mat22_base<T> mat = *this;
        set_columns(mat.row0, mat.row1);
    }
    mat22_base<T> get_transpose() const
    {
        return mat22_base<T>(
            xx, yx,
            xy, yy);
    }

    T get_trace() const { return xx + yy; }

    // is orthogonal if all columns are orthonormal
    bool is_orthogonal() const
    {
        return *this * get_transpose() == mat22_base<T>(
            1, 0,
            0, 1); // test that transpose == inverse
    }

    T get_determinant() const { return xx * yy - xy * yx; }
    bool has_inverse() const { !epsilon_equals<T>(get_determinant(), 0); }

    // finds inverse the hard way, use get_transpose() if you know the mat is orthogonal
    mat22_base<T> get_inverse() const
    {
        T s = 1 / get_determinant();
        return mat22_base<T>(
            yy * s, -xy * s,
            -yx * s, xx * s);
    }
    void invert() { *this = get_inverse(); }

    mat22_base<T> operator+() const { return { row0, row1 }; }
    mat22_base<T> operator-() const { return { -row0, -row1 }; }

    mat22_base<T> operator+(const mat22_base<T>& other) const { return { row0 + other.row0, row1 + other.row1 }; }
    mat22_base<T> operator-(const mat22_base<T>& other) const { return { row0 - other.row0, row1 - other.row1 }; }
    // NOT element-wise, this is normal matrix multiplication
    mat22_base<T> operator*(const mat22_base<T>& other) const
    {
        // TODO: test with benchmark and see if we need to implement strassen's multiplication
        mat22_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
        mat22_base<T> t = other.get_transpose();
        out.xx = dot(row0, t.row0);     out.xy = dot(row0, t.row1);
        out.yx = dot(row1, t.row0);     out.yy = dot(row1, t.row1);
#else
        out.xx = xx * other.xx + xy * other.yx;     out.xy = xx * other.xy + xy * other.yy;
        out.yx = yx * other.xx + yy * other.yx;     out.yy = yx * other.xy + yy * other.yy;
#endif
        return out;
    }
    vec2_base<T> operator*(const vec2_base<T>& other) const
    {
        vec2_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
        out.x = dot(row0, other);
        out.y = dot(row1, other);
#else
        out.x = xx * other.x + xy * other.y;
        out.y = yx * other.x + yy * other.y;
#endif
        return out;
    }
    std::vector<vec2_base<T>> operator*(const std::vector<vec2_base<T>>& other) const
    {
        return mat22_mul(*this, other);
    }
    // division doesn't make sense here
    mat22_base<T> operator+(const T& val) const { return { row0 + val, row1 + val }; }
    mat22_base<T> operator-(const T& val) const { return { row0 - val, row1 - val }; }
    mat22_base<T> operator*(const T& val) const { return { row0 * val, row1 * val }; }
    mat22_base<T> operator/(const T& val) const { return { row0 / val, row1 / val }; }

    mat22_base<T>& operator=(const mat22_base<T>& other) { row0 = other.row0; row1 = other.row1; return *this; }
    mat22_base<T>& operator+=(const mat22_base<T>& other) { row0 += other.row0; row1 += other.row1; return *this; }
    mat22_base<T>& operator-=(const mat22_base<T>& other) { row0 -= other.row0; row1 -= other.row1; return *this; }
    // *= and /= don't make sense here, *= specifically does not indicate if this is A * B or B * A
    mat22_base<T>& operator=(const T& val) { row0 = val; row1 = val; return *this; }
    mat22_base<T>& operator+=(const T& val) { row0 += val; row1 += val; return *this; }
    mat22_base<T>& operator-=(const T& val) { row0 -= val; row1 -= val; return *this; }
    mat22_base<T>& operator*=(const T& val) { row0 *= val; row1 *= val; return *this; }
    mat22_base<T>& operator/=(const T& val) { row0 /= val; row1 /= val; return *this; }

    inline vec2_base<T>& operator[](size_t idx) { return data[idx]; }
    inline const vec2_base<T>& operator[](size_t idx) const { return data[idx]; }
    inline bool operator==(const mat22_base<T>& other) const { return row0 == other.row0 && row1 == other.row1; }
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
inline constexpr mat22_base<T> identity22()
{
    return
    {
        1, 0,
        0, 1
    };
}
template <typename T>
void set_identity(mat22_base<T>& mat)
{
    mat.xx = 1; mat.xy = 0;
    mat.yx = 0; mat.yy = 1;
}
template <typename T>
inline mat22_base<T> gen_mat22_from_cols(const vec2_base<T>& col0, const vec2_base<T>& col1)
{
    return
    {
        col0.x, col1.x,
        col0.y, col1.y
    };
}
template <typename T>
void mat22_mul_in_place(const mat22_base<T>& mat, vec2_base<T> vecs[], size_t numVecs) // returns mat * vecs into vecs
{
    for(size_t i = 0; i < numVecs; i++)
    {
        vec2_base<T>& vec = vecs[i];
        vec2_base<T> v = vec;
#ifdef RQM_MATRIX_DOT_IMPL
        vec.x = dot(mat.row0, v);
        vec.y = dot(mat.row1, v);
#else
        vec.x = mat.xx * v.x + mat.xy * v.y;
        vec.y = mat.yx * v.x + mat.yy * v.y;
#endif
    }
}
template <typename T>
inline void mat22_mul_in_place(const mat22_base<T>& mat, std::vector<vec2_base<T>>& vecs) // returns mat * vecs into vecs
{
    mat22_mul_in_place(mat, vecs.data(), vecs.size());
}
template <typename T>
std::vector<vec2_base<T>> mat22_mul(const mat22_base<T>& mat, const std::vector<vec2_base<T>>& vecs) // returns mat * vecs
{
    // we don't use mat22_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
    size_t numVecs = vecs.size();
    std::vector<vec2_base<T>> out;
    out.reserve(numVecs);
    for(size_t i = 0; i < numVecs; i++)
    {
        const vec2_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
        out.push_back({ dot(mat.row0, vec), dot(mat.row1, vec) });
#else
        out.push_back({ mat.xx * vec.x + mat.xy * vec.y, mat.yx * vec.x + mat.yy * vec.y });
#endif
    }
    return out;
}


template <typename T>
union mat33_base
{
    struct { vec3_base<T> x; vec3_base<T> y; vec3_base<T> z; };
    struct { vec3_base<T> row0; vec3_base<T> row1; vec3_base<T> row2; };
    struct { T xx; T xy; T xz; T yx; T yy; T yz; T zx; T zy; T zz; };
    vec3_base<T> data[3]; // C++ is row major
    T flat_data[9];

    mat33_base() {}
    mat33_base(
        const T& val0, const T& val1, const T& val2,
        const T& val3, const T& val4, const T& val5,
        const T& val6, const T& val7, const T& val8)
        : xx(val0), xy(val1), xz(val2), yx(val3), yy(val4), yz(val5), zx(val6), zy(val7), zz(val8)
    {
    }
    mat33_base(const vec3_base<T>& vec0, const vec3_base<T>& vec1, const vec3_base<T>& vec2) : row0(vec0), row1(vec1), row2(vec2) {}
    mat33_base(const mat33_base<T>& mat) : row0(mat.row0), row1(mat.row1), row2(mat.row2) {}
    mat33_base(const mat22_base<T>& mat) : row0({ mat.row0, 0 }), row1({ mat.row1, 0 }), row2({ 0, 0 ,1 }) {}

    void set_columns(const vec3_base<T>& col0, const vec3_base<T>& col1, const vec3_base<T>& col2)
    {
        xx = col0.x; xy = col1.x; xz = col2.x;
        yx = col0.y; yy = col1.y; yz = col2.y;
        zx = col0.z; zy = col1.z; zz = col2.z;
    }

    void transpose()
    {
        mat33_base<T> mat = *this;
        set_columns(mat.row0, mat.row1, mat.row2);
    }
    mat33_base<T> get_transpose() const
    {
        return mat33_base<T>(
            xx, yx, zx,
            xy, yy, zy,
            xz, yz, zz);
    }

    T get_trace() const { return xx + yy + zz; }

    // is orthogonal if all columns are orthonormal
    bool is_orthogonal() const
    {
        return *this * get_transpose() == mat33_base<T>(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1); // test that transpose == inverse
    }

    T get_determinant() const { return xx * (yy * zz - yz * zy) - xy * (yx * zz - yz * zx) + xz * (yx * zy - yy * zx); }
    bool has_inverse() const { return !epsilon_equals<T>(get_determinant(), 0); }

    // finds inverse the hard way, use get_transpose() if you know the mat is orthogonal
    // TODO: potentially need to use Gaussian Elimination instead, idk
    mat33_base<T> get_inverse() const
    {
        T s = 1 / get_determinant();
        mat33_base<T> inv;
        inv.xx = s * (yy * zz - yz * zy);   inv.xy = s * (xz * zy - xy * zz);   inv.xz = s * (xy * yz - xz * yy);
        inv.yx = s * (yz * zx - yx * zz);   inv.yy = s * (xx * zz - xz * zx);   inv.yz = s * (xz * yx - xx * yz);
        inv.zx = s * (yx * zy - yy * zx);   inv.zy = s * (xy * zx - xx * zy);   inv.zz = s * (xx * yy - xy * yx);
        return inv; // TODO this needs testing!
    }
    void invert() { *this = get_inverse(); }

    mat33_base<T> operator+() const { return { row0, row1, row2 }; }
    mat33_base<T> operator-() const { return { -row0, -row1, -row2 }; }

    mat33_base<T> operator+(const mat33_base<T>& other) const { return { row0 + other.row0, row1 + other.row1, row2 + other.row2 }; }
    mat33_base<T> operator-(const mat33_base<T>& other) const { return { row0 - other.row0, row1 - other.row1, row2 - other.row2 }; }
    // NOT element-wise, this is normal matrix multiplication
    mat33_base<T> operator*(const mat33_base<T>& other) const
    {
        // TODO: test with benchmark and see if we need to implement strassen's multiplication
        mat33_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
        mat33_base<T> t = other.get_transpose();
        out.xx = dot(row0, t.row0);     out.xy = dot(row0, t.row1);     out.xz = dot(row0, t.row2);
        out.yx = dot(row1, t.row0);     out.yy = dot(row1, t.row1);     out.yz = dot(row1, t.row2);
        out.zx = dot(row2, t.row0);     out.zy = dot(row2, t.row1);     out.zz = dot(row2, t.row2);
#else
        out.xx = xx * other.xx + xy * other.yx + xz * other.zx;     out.xy = xx * other.xy + xy * other.yy + xz * other.zy;     out.xz = xx * other.xz + xy * other.yz + xz * other.zz;
        out.yx = yx * other.xx + yy * other.yx + yz * other.zx;     out.yy = yx * other.xy + yy * other.yy + yz * other.zy;     out.yz = yx * other.xz + yy * other.yz + yz * other.zz;
        out.zx = zx * other.xx + zy * other.yx + zz * other.zx;     out.zy = zx * other.xy + zy * other.yy + zz * other.zy;     out.zz = zx * other.xz + zy * other.yz + zz * other.zz;
#endif
        return out;
    }
    vec3_base<T> operator*(const vec3_base<T>& other) const
    {
        vec3_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
        out.x = dot(row0, other);
        out.y = dot(row1, other);
        out.z = dot(row2, other);
#else
        out.x = xx * other.x + xy * other.y + xz * other.z;
        out.y = yx * other.x + yy * other.y + yz * other.z;
        out.z = zx * other.x + zy * other.y + zz * other.z;
#endif
        return out;
    }
    std::vector<vec3_base<T>> operator*(const std::vector<vec3_base<T>>& other) const
    {
        return mat33_mul(*this, other);
    }
    // division doesn't make sense here
    mat33_base<T> operator+(const T& val) const { return { row0 + val, row1 + val, row2 + val }; }
    mat33_base<T> operator-(const T& val) const { return { row0 - val, row1 - val, row2 - val }; }
    mat33_base<T> operator*(const T& val) const { return { row0 * val, row1 * val, row2 * val }; }
    mat33_base<T> operator/(const T& val) const { return { row0 / val, row1 / val, row2 / val }; }

    mat33_base<T>& operator=(const mat33_base<T>& other) { row0 = other.row0; row1 = other.row1; row2 = other.row2; return *this; }
    mat33_base<T>& operator+=(const mat33_base<T>& other) { row0 += other.row0; row1 += other.row1; row2 += other.row2; return *this; }
    mat33_base<T>& operator-=(const mat33_base<T>& other) { row0 -= other.row0; row1 -= other.row1; row2 -= other.row2; return *this; }
    // *= and /= don't make sense here, *= specifically does not indicate if this is A * B or B * A
    mat33_base<T>& operator=(const T& val) { row0 = val; row1 = val; row2 = val; return *this; }
    mat33_base<T>& operator+=(const T& val) { row0 += val; row1 += val; row2 += val; return *this; }
    mat33_base<T>& operator-=(const T& val) { row0 -= val; row1 -= val; row2 -= val; return *this; }
    mat33_base<T>& operator*=(const T& val) { row0 *= val; row1 *= val; row2 *= val; return *this; }
    mat33_base<T>& operator/=(const T& val) { row0 /= val; row1 /= val; row2 /= val; return *this; }

    inline vec3_base<T>& operator[](size_t idx) { return data[idx]; }
    inline const vec3_base<T>& operator[](size_t idx) const { return data[idx]; }
    inline bool operator==(const mat33_base<T>& other) const { return row0 == other.row0 && row1 == other.row1 && row2 == other.row2; }
};
template <typename T>
std::ostream& operator<<(std::ostream& os, const mat33_base<T>& op1)
{
    os << '[' << op1.row0 << '|' << op1.row1 << '|' << op1.row2 << ']'; return os;
}
template <typename T>
inline mat33_base<T> operator+(const T& val, const mat33_base<T>& mat) { return mat + val; }
template <typename T>
inline mat33_base<T> operator-(const T& val, const mat33_base<T>& mat) { return -mat + val; }
template <typename T>
inline mat33_base<T> operator*(const T& val, const mat33_base<T>& mat) { return mat * val; }
template <typename T>
inline constexpr mat33_base<T> identity33()
{
    return
    {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
}
template <typename T>
void set_identity(mat33_base<T>& mat)
{
    mat.xx = 1; mat.xy = 0; mat.xz = 0;
    mat.yx = 0; mat.yy = 1; mat.yz = 0;
    mat.zx = 0; mat.zy = 0; mat.zz = 1;
}
template <typename T>
inline mat33_base<T> gen_mat33_from_cols(const vec3_base<T>& col0, const vec3_base<T>& col1, const vec3_base<T>& col2)
{
    return
    {
        col0.x, col1.x, col2.x,
        col0.y, col1.y, col2.y,
        col0.z, col1.z, col2.z
    };
}
template <typename T>
void mat33_mul_in_place(const mat33_base<T>& mat, vec3_base<T> vecs[], size_t numVecs) // returns mat * vecs into vecs
{
    for(size_t i = 0; i < numVecs; i++)
    {
        vec3_base<T>& vec = vecs[i];
        vec3_base<T> v = vec;
#ifdef RQM_MATRIX_DOT_IMPL
        vec.x = dot(mat.row0, v);
        vec.y = dot(mat.row1, v);
        vec.z = dot(mat.row2, v);
#else
        vec.x = mat.xx * v.x + mat.xy * v.y + mat.xz * v.z;
        vec.y = mat.yx * v.x + mat.yy * v.y + mat.yz * v.z;
        vec.z = mat.zx * v.x + mat.zy * v.y + mat.zz * v.z;
#endif
    }
}
// works with a list of vec2s
template <typename T>
void mat33_mul_in_place(const mat33_base<T>& mat, vec2_base<T> vecs[], size_t numVecs, const T& zval) // returns mat * vecs into vecs
{
    for(size_t i = 0; i < numVecs; i++)
    {
        vec2_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
        vec3_base<T> v = { vec, zval };
        vec.x = dot(mat.row0, v);
        vec.y = dot(mat.row1, v);
#else
        vec2_base<T> v = vec;
        vec.x = mat.xx * v.x + mat.xy * v.y + mat.xz * zval;
        vec.y = mat.yx * v.x + mat.yy * v.y + mat.yz * zval;
#endif
    }
}
template <typename T>
inline void mat33_mul_in_place(const mat33_base<T>& mat, std::vector<vec3_base<T>>& vecs) // returns mat * vecs into vecs
{
    mat33_mul_in_place(mat, vecs.data(), vecs.size());
}
// works with a list of vec2s
template <typename T>
inline void mat33_mul_in_place(const mat33_base<T>& mat, std::vector<vec2_base<T>>& vecs, const T& zval) // returns mat * vecs into vecs
{
    mat33_mul_in_place(mat, vecs.data(), vecs.size(), zval);
}
template <typename T>
std::vector<vec3_base<T>> mat33_mul(const mat33_base<T>& mat, const std::vector<vec3_base<T>>& vecs) // returns mat * vecs
{
    // we don't use mat33_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
    size_t numVecs = vecs.size();
    std::vector<vec3_base<T>> out;
    out.reserve(numVecs);
    for(size_t i = 0; i < numVecs; i++)
    {
        const vec3_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
        out.push_back({ dot(mat.row0, vec), dot(mat.row1, vec), dot(mat.row2, vec) });
#else
        out.push_back({ mat.xx * vec.x + mat.xy * vec.y + mat.xz * vec.z,
                        mat.yx * vec.x + mat.yy * vec.y + mat.yz * vec.z,
                        mat.zx * vec.x + mat.zy * vec.y + mat.zz * vec.z });
#endif
    }
    return out;
}
// works with a list of vec2s
template <typename T>
std::vector<vec2_base<T>> mat33_mul(const mat33_base<T>& mat, const std::vector<vec2_base<T>>& vecs, const T& zval) // returns mat * vecs
{
    // we don't use mat33_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
    size_t numVecs = vecs.size();
    std::vector<vec2_base<T>> out;
    out.reserve(numVecs);
    for(size_t i = 0; i < numVecs; i++)
    {
        const vec2_base<T>& vec = vecs[i];
        out.push_back({ mat.xx * vec.x + mat.xy * vec.y + mat.xz * zval,
                        mat.yx * vec.x + mat.yy * vec.y + mat.yz * zval });
    }
    return out;
}


template <typename T>
union mat44_base
{
    struct { vec4_base<T> x; vec4_base<T> y; vec4_base<T> z; vec4_base<T> w; };
    struct { vec4_base<T> row0; vec4_base<T> row1; vec4_base<T> row2; vec4_base<T> row3; };
    struct { T xx; T xy; T xz; T xw; T yx; T yy; T yz; T yw; T zx; T zy; T zz; T zw; T wx; T wy; T wz; T ww; };
    vec4_base<T> data[4]; // C++ is row major
    T flat_data[16];

    mat44_base() {}
    mat44_base(
        const T& val0, const T& val1, const T& val2, const T& val3,
        const T& val4, const T& val5, const T& val6, const T& val7,
        const T& val8, const T& val9, const T& val10, const T& val11,
        const T& val12, const T& val13, const T& val14, const T& val15)
        : xx(val0), xy(val1), xz(val2), xw(val3), yx(val4), yy(val5), yz(val6), yw(val7),
        zx(val8), zy(val9), zz(val10), zw(val11), wx(val12), wy(val13), wz(val14), ww(val15)
    {
    }
    mat44_base(const vec4_base<T>& vec0, const vec4_base<T>& vec1, const vec4_base<T>& vec2, const vec4_base<T>& vec3)
        : row0(vec0), row1(vec1), row2(vec2), row3(vec3)
    {
    }
    mat44_base(const mat44_base<T>& mat) : row0(mat.row0), row1(mat.row1), row2(mat.row2), row3(mat.row3) {}
    mat44_base(const mat33_base<T>& mat) : row0({ mat.row0, 0 }), row1(mat.row1, 0), row2(mat.row2, 0), row3({ 0, 0, 0, 1 }) {}

    void set_columns(const vec4_base<T>& col0, const vec4_base<T>& col1, const vec4_base<T>& col2, const vec4_base<T>& col3)
    {
        xx = col0.x; xy = col1.x; xz = col2.x; xw = col3.x;
        yx = col0.y; yy = col1.y; yz = col2.y; yw = col3.y;
        zx = col0.z; zy = col1.z; zz = col2.z; zw = col3.z;
        wx = col0.w; wy = col1.w; wz = col2.w; ww = col3.w;
    }

    void transpose()
    {
        mat44_base<T> mat = *this;
        set_columns(mat.row0, mat.row1, mat.row2, mat.row3);
    }
    mat44_base<T> get_transpose() const
    {
        return mat44_base<T>(
            xx, yx, zx, wx,
            xy, yy, zy, wy,
            xz, yz, zz, wz,
            xw, yw, zw, ww);
    }

    T get_trace() const { return xx + yy + zz + ww; }

    // is orthogonal if all columns are orthonormal
    bool is_orthogonal() const
    {
        return *this * get_transpose() == mat44_base<T>(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1); // test that transpose == inverse
    }

    // 22 multiplies
    T get_determinant() const // TODO pls test if correct
    {
        T p0, p1, p2, p3, p4, p5;
        p0 = zz * ww - zw * wz;
        p1 = zy * ww - zw * wy;
        p2 = zy * wz - zz * wy;
        p3 = zx * ww - zw * wx;
        p4 = zx * wz - zz * wx;
        p5 = zx * wy - zy * wx;
        return xx * (yy * p0 - yz * p1 + yw * p2) - xy * (yx * p0 - yz * p3 + yw * p4) + xz * (yx * p1 - yy * p3 + yw * p5) - xw * (yx * p2 - yy * p4 + yz * p5);
    }
    bool has_inverse() const { return !epsilon_equals<T>(get_determinant(), 0); }

    // finds inverse the hard way, use get_transpose() if you know the mat is orthogonal
    // TODO: potentially need to use Gaussian Elimination instead, idk
    // 122 multiplies
    mat44_base<T> get_inverse() const
    {
        T s = 1 / get_determinant();
        mat44_base<T> inv;
        T p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;
        p0 = zz * ww - zw * wz;
        p1 = zy * ww - zw * wy;
        p2 = zy * wz - zz * wy;
        p3 = yz * ww - yw * wz;
        p4 = yy * ww - yw * wy;
        p5 = yy * wz - yz * wy;
        p6 = yz * zw - yw * zz;
        p7 = yy * zw - yw * zy;
        p8 = yy * zz - yz * zy;
        p9 = zx * ww - zw * wx;
        p10 = zx * wz - zz * wx;
        p11 = yx * ww - yw * wx;
        p12 = yx * wz - yz * wx;
        p13 = yx * zw - yw * zx;
        p14 = yx * zz - yz * zx;
        p15 = zx * wy - zy * wx;
        p16 = yx * wy - yy * wx;
        p17 = yx * zy - yy * zx;
        inv.xx = s * (yy * p0 - yz * p1 + yw * p2);
        inv.xy = s * (xz * p1 - xy * p0 - xw * p2);
        inv.xz = s * (xy * p3 - xz * p4 + xw * p5);
        inv.xw = s * (xz * p7 - xy * p6 - xw * p8);
        inv.yx = s * (yz * p9 - yx * p0 - yw * p10);
        inv.yy = s * (xx * p0 - xz * p9 + xw * p10);
        inv.yz = s * (xz * p11 - xx * p3 - xw * p12);
        inv.yw = s * (xx * p6 - xz * p13 + xw * p14);
        inv.zx = s * (yx * p1 - yy * p9 + yw * p15);
        inv.zy = s * (xy * p9 - xx * p1 - xw * p15);
        inv.zz = s * (xx * p4 - xy * p11 + xw * p16);
        inv.zw = s * (xy * p13 - xx * p7 - xw * p17);
        inv.wx = s * (yy * p10 - yx * p2 - yz * p15);
        inv.wy = s * (xx * p2 - xy * p10 + xz * p15);
        inv.wz = s * (xy * p12 - xx * p5 - xz * p16);
        inv.ww = s * (xx * p8 - xy * p14 + xz * p17);
        return inv; // TODO this needs testing!
    }
    void invert() { *this = get_inverse(); }

    mat44_base<T> operator+() const { return { row0, row1, row2, row3 }; }
    mat44_base<T> operator-() const { return { -row0, -row1, -row2, -row3 }; }

    mat44_base<T> operator+(const mat44_base<T>& other) const { return { row0 + other.row0, row1 + other.row1, row2 + other.row2, row3 + other.row3 }; }
    mat44_base<T> operator-(const mat44_base<T>& other) const { return { row0 - other.row0, row1 - other.row1, row2 - other.row2, row3 - other.row3 }; }
    // NOT element-wise, this is normal matrix multiplication
    mat44_base<T> operator*(const mat44_base<T>& other) const
    {
        // TODO: test with benchmark and see if we need to implement strassen's multiplication
        mat44_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
        mat44_base<T> t = other.get_transpose();
        out.xx = dot(row0, t.row0);     out.xy = dot(row0, t.row1);     out.xz = dot(row0, t.row2);     out.xw = dot(row0, t.row3);
        out.yx = dot(row1, t.row0);     out.yy = dot(row1, t.row1);     out.yz = dot(row1, t.row2);     out.yw = dot(row1, t.row3);
        out.zx = dot(row2, t.row0);     out.zy = dot(row2, t.row1);     out.zz = dot(row2, t.row2);     out.zw = dot(row2, t.row3);
        out.wx = dot(row3, t.row0);     out.wy = dot(row3, t.row1);     out.wz = dot(row3, t.row2);     out.ww = dot(row3, t.row3);
#else
        out.xx = xx * other.xx + xy * other.yx + xz * other.zx + xw * other.wx;   out.xy = xx * other.xy + xy * other.yy + xz * other.zy + xw * other.wy;   out.xz = xx * other.xz + xy * other.yz + xz * other.zz + xw * other.wz;   out.xw = xx * other.xw + xy * other.yw + xz * other.zw + xw * other.ww;
        out.yx = yx * other.xx + yy * other.yx + yz * other.zx + yw * other.wx;   out.yy = yx * other.xy + yy * other.yy + yz * other.zy + yw * other.wy;   out.yz = yx * other.xz + yy * other.yz + yz * other.zz + yw * other.wz;   out.yw = yx * other.xw + yy * other.yw + yz * other.zw + yw * other.ww;
        out.zx = zx * other.xx + zy * other.yx + zz * other.zx + zw * other.wx;   out.zy = zx * other.xy + zy * other.yy + zz * other.zy + zw * other.wy;   out.zz = zx * other.xz + zy * other.yz + zz * other.zz + zw * other.wz;   out.zw = zx * other.xw + zy * other.yw + zz * other.zw + zw * other.ww;
        out.wx = wx * other.xx + wy * other.yx + wz * other.zx + ww * other.wx;   out.wy = wx * other.xy + wy * other.yy + wz * other.zy + ww * other.wy;   out.wz = wx * other.xz + wy * other.yz + wz * other.zz + ww * other.wz;   out.ww = wx * other.xw + wy * other.yw + wz * other.zw + ww * other.ww;
#endif
        return out;
    }
    vec4_base<T> operator*(const vec4_base<T>& other) const
    {
        vec4_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
        out.x = dot(row0, other);
        out.y = dot(row1, other);
        out.z = dot(row2, other);
        out.w = dot(row3, other);
#else
        out.x = xx * other.x + xy * other.y + xz * other.z + xw * other.w;
        out.y = yx * other.x + yy * other.y + yz * other.z + yw * other.w;
        out.z = zx * other.x + zy * other.y + zz * other.z + zw * other.w;
        out.w = wx * other.x + wy * other.y + wz * other.z + ww * other.w;
#endif
        return out;
    }
    std::vector<vec4_base<T>> operator*(const std::vector<vec4_base<T>>& other) const
    {
        return mat44_mul(*this, other);
    }
    // division doesn't make sense here
    mat44_base<T> operator+(const T& val) const { return { row0 + val, row1 + val, row2 + val, row3 + val }; }
    mat44_base<T> operator-(const T& val) const { return { row0 - val, row1 - val, row2 - val, row3 - val }; }
    mat44_base<T> operator*(const T& val) const { return { row0 * val, row1 * val, row2 * val, row3 * val }; }
    mat44_base<T> operator/(const T& val) const { return { row0 / val, row1 / val, row2 / val, row3 / val }; }

    mat44_base<T>& operator=(const mat44_base<T>& other) { row0 = other.row0; row1 = other.row1; row2 = other.row2; row3 = other.row3; return *this; }
    mat44_base<T>& operator+=(const mat44_base<T>& other) { row0 += other.row0; row1 += other.row1; row2 += other.row2; row3 += other.row3; return *this; }
    mat44_base<T>& operator-=(const mat44_base<T>& other) { row0 -= other.row0; row1 -= other.row1; row2 -= other.row2; row3 -= other.row3; return *this; }
    // *= and /= don't make sense here, *= specifically does not indicate if this is A * B or B * A
    mat44_base<T>& operator=(const T& val) { row0 = val; row1 = val; row2 = val; row3 = val; return *this; }
    mat44_base<T>& operator+=(const T& val) { row0 += val; row1 += val; row2 += val; row3 += val; return *this; }
    mat44_base<T>& operator-=(const T& val) { row0 -= val; row1 -= val; row2 -= val; row3 -= val; return *this; }
    mat44_base<T>& operator*=(const T& val) { row0 *= val; row1 *= val; row2 *= val; row3 *= val; return *this; }
    mat44_base<T>& operator/=(const T& val) { row0 /= val; row1 /= val; row2 /= val; row3 /= val; return *this; }

    inline vec4_base<T>& operator[](size_t idx) { return data[idx]; }
    inline const vec4_base<T>& operator[](size_t idx) const { return data[idx]; }
    inline bool operator==(const mat44_base<T>& other) const { return row0 == other.row0 && row1 == other.row1 && row2 == other.row2 && row3 == other.row3; }
};
template <typename T>
std::ostream& operator<<(std::ostream& os, const mat44_base<T>& op1)
{
    os << '[' << op1.row0 << '|' << op1.row1 << '|' << op1.row2 << '|' << op1.row3 << ']'; return os;
}
template <typename T>
inline mat44_base<T> operator+(const T& val, const mat44_base<T>& mat) { return mat + val; }
template <typename T>
inline mat44_base<T> operator-(const T& val, const mat44_base<T>& mat) { return -mat + val; }
template <typename T>
inline mat44_base<T> operator*(const T& val, const mat44_base<T>& mat) { return mat * val; }
template <typename T>
inline constexpr mat44_base<T> identity44()
{
    return
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}
template <typename T>
void set_identity(mat44_base<T>& mat)
{
    mat.xx = 1; mat.xy = 0; mat.xz = 0; mat.xw = 0;
    mat.yx = 0; mat.yy = 1; mat.yz = 0; mat.yw = 0;
    mat.zx = 0; mat.zy = 0; mat.zz = 1; mat.zw = 0;
    mat.wx = 0; mat.wy = 0; mat.wz = 0; mat.ww = 1;
}
template <typename T>
inline mat44_base<T> gen_mat44_from_cols(const vec4_base<T>& col0, const vec4_base<T>& col1, const vec4_base<T>& col2, const vec4_base<T>& col3)
{
    return
    {
        col0.x, col1.x, col2.x, col3.x,
        col0.y, col1.y, col2.y, col3.y,
        col0.z, col1.z, col2.z, col3.z,
        col0.w, col1.w, col2.w, col3.w
    };
}
template <typename T>
void mat44_mul_in_place(const mat44_base<T>& mat, vec4_base<T> vecs[], size_t numVecs) // returns mat * vecs into vecs
{
    for(size_t i = 0; i < numVecs; i++)
    {
        vec4_base<T>& vec = vecs[i];
        vec4_base<T> v = vec;
#ifdef RQM_MATRIX_DOT_IMPL
        vec.x = dot(mat.row0, v);
        vec.y = dot(mat.row1, v);
        vec.z = dot(mat.row2, v);
        vec.w = dot(mat.row3, v);
#else
        vec.x = mat.xx * v.x + mat.xy * v.y + mat.xz * v.z + mat.xw * v.w;
        vec.y = mat.yx * v.x + mat.yy * v.y + mat.yz * v.z + mat.yw * v.w;
        vec.z = mat.zx * v.x + mat.zy * v.y + mat.zz * v.z + mat.zw * v.w;
        vec.w = mat.wx * v.x + mat.wy * v.y + mat.wz * v.z + mat.ww * v.w;
#endif
    }
}
// works with a list of vec3s
template <typename T>
void mat44_mul_in_place(const mat44_base<T>& mat, vec3_base<T> vecs[], size_t numVecs, const T& wval) // returns mat * vecs into vecs
{
    for(size_t i = 0; i < numVecs; i++)
    {
        vec3_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
        vec4_base<T> v = { vec, wval };
        vec.x = dot(mat.row0, v);
        vec.y = dot(mat.row1, v);
        vec.z = dot(mat.row2, v);
#else
        vec3_base<T> v = vec;
        vec.x = mat.xx * v.x + mat.xy * v.y + mat.xz * v.z + mat.xw * wval;
        vec.y = mat.yx * v.x + mat.yy * v.y + mat.yz * v.z + mat.yw * wval;
        vec.z = mat.zx * v.x + mat.zy * v.y + mat.zz * v.z + mat.zw * wval;
#endif
    }
}
template <typename T>
inline void mat44_mul_in_place(const mat44_base<T>& mat, std::vector<vec4_base<T>>& vecs) // returns mat * vecs into vecs
{
    mat44_mul_in_place(mat, vecs.data(), vecs.size());
}
// works with a list of vec2s
template <typename T>
inline void mat44_mul_in_place(const mat44_base<T>& mat, std::vector<vec3_base<T>>& vecs, const T& wval) // returns mat * vecs into vecs
{
    mat44_mul_in_place(mat, vecs.data(), vecs.size(), wval);
}
template <typename T>
std::vector<vec4_base<T>> mat44_mul(const mat44_base<T>& mat, const std::vector<vec4_base<T>>& vecs) // returns mat * vecs
{
    // we don't use mat44_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
    size_t numVecs = vecs.size();
    std::vector<vec4_base<T>> out;
    out.reserve(numVecs);
    for(size_t i = 0; i < numVecs; i++)
    {
        const vec4_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
        out.push_back({ dot(mat.row0, vec), dot(mat.row1, vec), dot(mat.row2, vec), dot(mat.row3, vec) });
#else
        out.push_back({ mat.xx * vec.x + mat.xy * vec.y + mat.xz * vec.z + mat.xw * vec.w,
                        mat.yx * vec.x + mat.yy * vec.y + mat.yz * vec.z + mat.yw * vec.w,
                        mat.zx * vec.x + mat.zy * vec.y + mat.zz * vec.z + mat.zw * vec.w,
                        mat.wx * vec.x + mat.wy * vec.y + mat.wz * vec.z + mat.ww * vec.w });
#endif
    }
    return out;
}
// works with a list of vec3s
template <typename T>
std::vector<vec3_base<T>> mat44_mul(const mat44_base<T>& mat, const std::vector<vec3_base<T>>& vecs, const T& wval) // returns mat * vecs
{
    // we don't use mat44_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
    size_t numVecs = vecs.size();
    std::vector<vec3_base<T>> out;
    out.reserve(numVecs);
    for(size_t i = 0; i < numVecs; i++)
    {
        const vec3_base<T>& vec = vecs[i];
        out.push_back({ mat.xx * vec.x + mat.xy * vec.y + mat.xz * vec.z + mat.xw * wval,
                        mat.yx * vec.x + mat.yy * vec.y + mat.yz * vec.z + mat.yw * wval,
                        mat.zx * vec.x + mat.zy * vec.y + mat.zz * vec.z + mat.zw * wval });
    }
    return out;
}


typedef mat22_base<float> mat22;
typedef mat33_base<float> mat33;
typedef mat44_base<float> mat44;

typedef mat22_base<double> dmat22;
typedef mat33_base<double> dmat33;
typedef mat44_base<double> dmat44;


}